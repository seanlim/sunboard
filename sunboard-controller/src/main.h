#ifndef main_h
#define main_h

#include <Arduino.h>
#include <NeoPixelBus.h>
void ButtonTaskCode(void *pvParams);

void printString(String s);
void parseLine(String data);
void parseRoute(String data);
void processRouteSegment(String subStr);

void handleEncoderRotate(long val);

#endif
