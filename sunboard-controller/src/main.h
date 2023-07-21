#ifndef main_h
#define main_h

#include <Arduino.h>
#include <NeoPixelBus.h>
void ButtonTaskCode(void *pvParams);

void parseRoute(String data);
void processSubStr(String subStr);

#endif
