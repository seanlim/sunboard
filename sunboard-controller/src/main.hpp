#include <Arduino.h>
#include <NeoPixelBus.h>
void ButtonTaskCode(void *pvParams);

int state();

void setRow(uint8_t row);
int setColumn(uint8_t col);
void setPixel(uint8_t row, uint8_t col, RgbColor color);