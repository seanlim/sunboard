#include <Arduino.h>
#include <NeoPixelBus.h>

const int BUTT_INPUT = 4;
const int ROWS[BUTT_INPUT] = {17, 16, 27, 13};
const int COLS[BUTT_INPUT] = {26, 25, 22, 21};
const int STATE_PIN = 23;
const int PIXEL_PIN = 4;
const int PIXEL_COUNT = 154;

#define colorSaturation 128

int PREV_ROW_STATE[BUTT_INPUT] = {0, 0, 0, 0};
int PREV_COL_STATE[BUTT_INPUT] = {0, 0, 0, 0};

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PIXEL_COUNT, PIXEL_PIN);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

int loopCount = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // initialize LEDs
  /*
  strip.Begin();
  strip.Show();
  Serial.println("LEDs Initialized");
  */

  // initialize rows;
  for (int i = 0; i < BUTT_INPUT; i++)
  {
    pinMode(ROWS[i], OUTPUT);
    pinMode(COLS[i], OUTPUT);
  }

  pinMode(STATE_PIN, OUTPUT);

  Serial.println("Setup Done");
}

void loop()
{
  Serial.println("looping!");

  // SET row 0 to high
  digitalWrite(ROWS[0], HIGH);
  digitalWrite(ROWS[1], HIGH);
  digitalWrite(ROWS[2], HIGH);
  digitalWrite(ROWS[3], HIGH);

  for (int i = 0; i < BUTT_INPUT; i++)
  {
    digitalWrite(COLS[i], HIGH);
    int colVal = digitalRead(STATE_PIN);
    digitalWrite(COLS[i], LOW);
    Serial.printf("COL PIN %d,before: %d,  after: %d\n", i + 1, PREV_COL_STATE[i], colVal);
    PREV_COL_STATE[i] = colVal;
  }

  loopCount++;
  delay(1000);
}