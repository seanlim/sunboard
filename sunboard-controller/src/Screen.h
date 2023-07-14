#ifndef Screen_h
#define Screen_h

#include <Adafruit_SSD1306.h>
#include <Wire.h>

const int SCL_PIN = 18; // also SCK
const int SDA_PIN = 19;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 32;
const int OLED_RESET = -1;
const int SCREEN_ADDRESS = 0x3C;

class Screen
{
private:
    Adafruit_SSD1306 display;
    TwoWire wire;

public:
    void setup();
    void setText(const char *fmt...);
    void render();
};

#endif