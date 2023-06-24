#include <Arduino.h>
#include <NeoPixelBus.h>

#ifdef TRACE
#define TRACEF(...) Serial.printf(__VA_ARGS__)
#else
#define TRACEF(...)
#endif

#define DEBUG 1

#ifdef DEBUG
#define DEBUGF(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUGF(...)
#endif

const int NUM_BUTT_INPUT_PINS = 4;
const int ROWS_PINS[NUM_BUTT_INPUT_PINS] = {17, 16, 27, 13};
const int COLS_PINS[NUM_BUTT_INPUT_PINS] = {22, 21, 26, 25};
const int STATE_PIN = 23;
const int PIXEL_PIN = 4;

const int NUM_ROWS = 14;
const int NUM_COLS = 11;
const int PIXEL_COUNT = 154;

#define colorSaturation 128

int PREV_STATE[NUM_ROWS * NUM_COLS] = {};

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PIXEL_COUNT, PIXEL_PIN);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // initialize LEDs
  strip.Begin();
  strip.Show();
  Serial.println("LEDs Initialized");

  // initialize rows;
  for (int i = 0; i < NUM_BUTT_INPUT_PINS; i++)
  {
    pinMode(ROWS_PINS[i], OUTPUT);
    pinMode(COLS_PINS[i], OUTPUT);
  }

  pinMode(STATE_PIN, INPUT_PULLDOWN);

  Serial.println("Setup Done");
}

int state()
{
  delay(1);
  return digitalRead(STATE_PIN);
}
// set the pins on the multiplexer for the given row. Since the PCB is connected from pin 2 to 15, this has to be added by 2.
// so for example, if i want to activate the 0th row, i have to send "2" to the multiplexer.
void setRow(uint8_t row)
{
  TRACEF("setting row: %d: ", row + 1);
  for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  {
    int val = ((row + 2) & (1 << pin)) >> pin;
    TRACEF("%d ", val);
    digitalWrite(ROWS_PINS[pin], val);
  }
  TRACEF("\n");
}

int setColumn(uint8_t col)
{
  TRACEF("setting col: %d: ", col + 1);
  for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  {
    int val = (col & (1 << pin)) >> pin;
    TRACEF("%d ", val);
    digitalWrite(COLS_PINS[pin], val);
  }
  TRACEF("\n");
  return state();
}

void setPixel(uint8_t row, uint8_t col, RgbColor color)
{
  int index = (13 - row) * 11 + col;
  DEBUGF("Lighting up %d\n", index);
  strip.SetPixelColor(index, color);
}

void loop()
{
  for (uint8_t row = 0; row < NUM_ROWS; row++)
  {
    setRow(row);
    for (uint8_t col = 0; col < NUM_COLS; col++)
    {
      int result = setColumn(col);
      if (result)
      {
        setPixel(row, col, red);
        DEBUGF("%d %d pressed\n", row + 1, col + 1);
      }
    }
  }

  strip.Show();
}