#include "Encoder.h"

void Encoder::setup()
{
    encoder.attachFullQuad(KNOB_A_PIN, KNOB_B_PIN);
    encoder.setCount(0);
}

void Encoder::onChange(void (*cb)(long))
{
    long current = encoder.getCount() / 4;
    if (current != previousPosition)
    {
        cb(current);
        previousPosition = current;
    }
}
