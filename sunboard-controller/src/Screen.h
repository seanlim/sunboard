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
    TwoWire wire = TwoWire(0);
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &wire, OLED_RESET);

public:
    void setup();
    void setText(const char *fmt);
};

#endif