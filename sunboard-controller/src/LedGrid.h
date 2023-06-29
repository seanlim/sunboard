#ifndef LedGrid_h
#define LedGrid_h

#include <NeoPixelBus.h>
#include <NeoPixelBus.h>

#include "logging.h"

#define colorSaturation 128

const int PIXEL_COUNT = 154;
const int PIXEL_PIN = 4;

const RgbColor black(0);
const RgbColor red(colorSaturation, 0, 0);
const RgbColor green(0, colorSaturation, 0);
const RgbColor blue(0, 0, colorSaturation);
const RgbColor white(colorSaturation);

class LedGrid
{
private:
    RgbColor state[PIXEL_COUNT];
    NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip;

public:
    LedGrid() : strip(PIXEL_COUNT, PIXEL_PIN), state{black} {}

    void setup();

    void clear();

    void setPixel(int row, int col, RgbColor c);

    void setPixel(int index, RgbColor c);

    void render();
};

#endif