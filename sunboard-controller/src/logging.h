#ifndef logging_h
#define logging_h

#include <Arduino.h>

#define DEBUG 1

#ifdef TRACE
#define TRACEF(...) Serial.printf(__VA_ARGS__)
#else
#define TRACEF(...)
#endif

#ifdef DEBUG
#define DEBUGF(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUGF(...)
#endif

#endif