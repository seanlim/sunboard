#include "main.h"

#include <Arduino.h>
#include <BLESerial.h>
#include <BLEDevice.h>

#include "LedGrid.h"
#include "buttonGrid.h"
#include "logging.h"
#include "Screen.h"
#include "Encoder.h"

ButtonGrid buttons;
LedGrid leds;
Screen screen;
Encoder encoder;

// TaskHandle_t ButtonTask;

BLESerial bleReceive;

const int MAX_ROUTES = 10;
char bleReceiveName[] = "Sunboard Receiver";
char bleSendName[] = "Sunboard Sender";
char moonboardName[] = "Moonboard";
String routes[MAX_ROUTES];
int numRoutes = 0;

static BLEUUID serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID characteristicUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
static boolean doConnect = false;
static boolean connected = false;

static BLEAddress *pServerAddress;
static BLERemoteCharacteristic *txCharacteristic;

bool connectToServer(BLEAddress pAddress)
{
  BLEClient *pClient = BLEDevice::createClient();
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);

  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }

  txCharacteristic = pRemoteService->getCharacteristic(characteristicUUID);
  if (txCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID");
    Serial.println(characteristicUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristics");
  return true;
}

class MyAdvertiseDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.getName() == moonboardName)
    {
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      Serial.println("Device Found. Connecting!");
    }
  }
};

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // initialize LEDs
  leds.setup();
  buttons.setup();
  encoder.setup();
  screen.setup();

  bleReceive.begin(bleReceiveName);

  BLEDevice::init(bleSendName);
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertiseDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  Serial.println("Setup Done");

  // xTaskCreate(ButtonTaskCode, "ButtonTask", 10000, NULL, 1, &ButtonTask);
}

void ButtonTaskCode(void *pvParams)
{
  while (true)
  {
    buttons.parseButtonState();
  }
}

// set the pins on the multiplexer for the given row. Since the PCB is connected from pin 2 to 15, this has to be added by 2.
// so for example, if i want to activate the 0th row, i have to send "2" to the multiplexer.

void handleEncoderRotate(long val)
{
  // char buffer[40];
  // sprintf(buffer, "Encoder: %d", val);
  parseLine(routes[val % MAX_ROUTES]);
}

bool RecConnected = false;

String receivedData = "";

void loop()
{
  encoder.onChange(&handleEncoderRotate);
  if (bleReceive.connected())
  {
    if (!RecConnected)
    {
      Serial.println("connected!");
      RecConnected = true;
    }
    if (bleReceive.available())
    {
      char data;
      while (bleReceive.available())
      {
        data = bleReceive.read();
        receivedData.concat(data);
        if (receivedData[receivedData.length() - 1] == '|')
        {
          if (numRoutes < MAX_ROUTES)
          {
            routes[numRoutes] = receivedData;
            numRoutes++;
          }
          parseLine(receivedData);
          receivedData = "";
        }
      }
    }
  }
  else
  {
    if (RecConnected)
    {
      Serial.println("Disconnected");
      RecConnected = false;
    }
  }

  if (doConnect == true)
  {
    if (connectToServer(*pServerAddress))
    {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    }
    else
    {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
    }
    doConnect = false;
  }

  leds.render();
}

void printString(String s)
{
  for (char c : s)
  {
    Serial.printf("%c", c);
  }
  Serial.println();
}

void parseLine(String data)
{
  if (data.length() == 0)
    return;
  TRACEF("Received of len %d:\n", data.length());
  String title;
  String grade;
  String route;
  int seperatorCount = 0;
  int gradeStart = 0;
  int routeStart = 0;
  for (int i = 0; i < data.length(); i++)
  {
    if (data[i] == '%')
    {
      seperatorCount++;
      if (seperatorCount == 1)
      {
        title = data.substring(0, i);
        gradeStart = i;
      }
      if (seperatorCount == 2)
      {
        grade = data.substring(gradeStart + 1, i);
        routeStart = i;
        break;
      }
    }
  }
  route = data.substring(routeStart);

  if (txCharacteristic != nullptr)
  {
    Serial.printf("Can it write?: %i", txCharacteristic->canWrite() ? 1 : 0);
    txCharacteristic->writeValue(route.substring(0, route.length()).c_str());
  }
  route = route.substring(3, route.length() - 2);
  title.concat('\n');
  title.concat(grade);
  screen.setText(title.c_str());

  parseRoute(route);
}

void parseRoute(String data)
{
  leds.clear();
  String subStr;
  for (int i = 0; i < data.length(); i++)
  {
    if (data[i] == ',')
    {
      // process substring
      processRouteSegment(subStr);
      subStr = "";
      continue;
    }
    // build string until comma
    subStr.concat(data[i]);
  }
  processRouteSegment(subStr);
  leds.render();
}

void processRouteSegment(String subStr)
{
  RgbColor color;
  if (subStr[0] == 'S')
    color = green;
  if (subStr[0] == 'P')
    color = blue;
  if (subStr[0] == 'E')
    color = red;

  int pos = subStr.substring(1).toInt();
  int column = pos / 18;
  int row = (pos % 18);
  if (column % 2 == 1)
  {
    row = 17 - row;
  }
  row = row - 4;
  TRACEF("%s: %d %d %d\n", subStr, pos, row, column);
  leds.setPixel(row, column, color);
}
