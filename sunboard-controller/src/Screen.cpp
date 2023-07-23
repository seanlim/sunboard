#include "Screen.h"
#include "logging.h"
#include <Wire.h>

void Screen::setup()
{
    while (!wire.begin(SDA_PIN, SCL_PIN))
    {
        TRACEF("Waitin for wire to initialize");
        // do nothing while waiting for this to initialize
    }
    TRACEF("Wire initialized");
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println("DISPLAY Allocation Failed");
        for (;;)
            ; // stop the rest from running
    }
    TRACEF("Display initialized");
    display.setRotation(2);
}

void Screen::setText(const char *fmt)
{
    setText(fmt, 0, 0);
}

void Screen::setText(const char *fmt, int x, int y)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(fmt);
    display.display();
}
