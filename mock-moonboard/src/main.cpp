#include <Arduino.h>
#include "BLESerial.h"

BLESerial bleSerial;
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

bool connected;

void setup()
{
  Serial.begin(115200);
  char bleName[] = "Moonboard";
  bleSerial.begin(bleName);
  Serial.println("-----------------");
  Serial.println("Waiting for the mobile app to connect ...");
  Serial.println("-----------------");
  // put your setup code here, to run once:
}

void loop()
{
  if (bleSerial.connected())

  {
    while (bleSerial.available())
    {
      char c = bleSerial.read();
      Serial.print(c);
      /* code */
    }
  }
  // put your main code here, to run repeatedly:
}