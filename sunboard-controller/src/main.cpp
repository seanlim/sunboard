#include "main.h"

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <BLESerial.h>
#include <Adafruit_SSD1306.h>

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

  char bleReceiveName[] = "Sunboard Receiver";
  char bleSendName[] = "Sunboard Sender";
  bleReceive.begin(bleReceiveName);
  bleSend.begin(bleSendName);

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
  char buffer[40];
  Serial.printf("Encoder: %d\n", val);
  sprintf(buffer, "Encoder: %d", val);
  screen.setText(buffer);
}

void loop()
{

  encoder.onChange(&handleEncoderRotate);
  if (bleReceive.connected())
  {
    if (bleReceive.available())
    {
      String result;
      char data;
      while (bleReceive.available())
      {
        leds.clear();
        data = bleReceive.read();
        result.concat(data);
      }
      parseRoute(result);
    }
  }
  leds.render();
}

// String asdf = "l#S175,P85,P152,E126#";

void parseRoute(String data)
{
  Serial.printf("data: %s\n", data);
  // remove starting and ending characters
  data = data.substring(2);
  data.remove(data.length() - 1);
  String subStr;
  for (int i = 0; i < data.length(); i++)
  {
    if (data[i] == ',')
    {
      // process substring
      processSubStr(subStr);
      subStr = "";
      continue;
    }
    // build string until comma
    subStr.concat(data[i]);
  }
  processSubStr(subStr);
}

void processSubStr(String subStr)
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
  Serial.printf("%s: %d %d %d\n", subStr, pos, row, column);
  leds.setPixel(row, column, color);
}
