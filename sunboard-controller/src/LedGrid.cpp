#include "LedGrid.h"

void LedGrid::setup()
{
    strip.Begin();
    strip.Show();
}

void LedGrid::clear()
{
    for (int i = 0; i < PIXEL_COUNT; i++)
        state[i] = black;
    Serial.println("LEDs Initialized");
}

void LedGrid::setPixel(int row, int col, RgbColor c)
{
    if (row < 0 || col < 0)
        return;
    int index = (13 - row) * 11 + col;
    TRACEF("Lighting up %d\n", index);
    state[index] = c;
}
void LedGrid::setPixel(int index, RgbColor c)
{
    state[index] = c;
}

void LedGrid::render()
{
    for (int i = 0; i < PIXEL_COUNT; i++)
    {
        strip.SetPixelColor(i, state[i]);
    }
    strip.Show();
}