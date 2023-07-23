#include "main.h"

#include <Arduino.h>
#include <BLESerial.h>

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
BLESerial bleSend;

const int MAX_ROUTES = 10;
char bleReceiveName[] = "Sunboard Receiver";
char bleSendName[] = "Sunboard Sender";
String routes[MAX_ROUTES];
int numRoutes = 0;
int routeIndex = 0;

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

bool connected = false;

String receivedData = "";

void loop()
{
  encoder.onChange(&handleEncoderRotate);
  if (bleReceive.connected())
  {
    if (!connected)
    {
      Serial.println("connected!");
      connected = true;
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
    if (connected)
    {
      Serial.println("Disconnected");
      connected = false;
    }
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
  route = route.substring(3, route.length() - 2);
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
