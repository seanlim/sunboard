#include "Screen.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

void Screen::setup()
{
    wire.begin(SDA_PIN, SCL_PIN);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("DISPLAY Allocation Failed"));
        for (;;)
            ; // stop the rest from running
    }
    display.println("SUNBOARD READY FOR USE");
    display.display();
    delay(100);
}

void Screen::setText(const char *fmt...)
{
    display.printf(fmt);
}

void Screen::render()
{
    display.display();
}
