#ifndef Encoder_h
#define Encoder_h

const int KNOB_A_PIN = 33;
const int KNOB_B_PIN = 32;

#include <ESP32Encoder.h>

#endif

class Encoder
{
private:
    ESP32Encoder encoder;
    long previousPosition = -999;

public:
    void setup();
    void onChange(void (*cb)(long));
};