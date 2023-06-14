#include <Arduino.h>
#include <NeoPixelBus.h>

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

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

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

int prev = 99;

void loop()
{
  digitalWrite(ROWS_PINS[0], 0);
  digitalWrite(ROWS_PINS[1], 1);
  digitalWrite(ROWS_PINS[2], 0);
  digitalWrite(ROWS_PINS[3], 0);

  digitalWrite(COLS_PINS[0], 0);
  digitalWrite(COLS_PINS[1], 0);
  digitalWrite(COLS_PINS[2], 0);
  digitalWrite(COLS_PINS[3], 0);

  for (int i = 0; i < NUM_COLS; i++)
  {
    for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
    {
      digitalWrite(COLS_PINS[pin], (i & 1 << pin) >> pin);
    }
    int state = digitalRead(STATE_PIN);
    Serial.printf("%d ", state);
    delay(50);
  }
  Serial.printf("\n\n");

  // iterate through each row
  // for (uint8_t row = 0; row < NUM_ROWS; row++)
  // {
  //   // set row pins
  //   for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  //   {
  //     digitalWrite(ROWS_PINS[pin], ((row + 2) & 1 << pin) >> pin);
  //   }
  //   // iterate through each column
  //   for (uint8_t col = 0; col < NUM_COLS; col++)
  //   {
  //     // set col pins
  //     for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  //     {
  //       digitalWrite(COLS_PINS[pin], (col & 1 << pin) >> pin);
  //     }
  //     // read state
  //     int button_pressed = digitalRead(STATE_PIN);
  //     if (PREV_STATE[row * NUM_COLS + col] != button_pressed)
  //     {
  //       // Serial.printf("[%d][%d] = %d\n", row, col, button_pressed);
  //       PREV_STATE[row * NUM_COLS + col] = button_pressed;
  //     }
  //     for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  //     {
  //       digitalWrite(COLS_PINS[pin], 0);
  //     }
  //     Serial.printf("%d ", button_pressed);
  //   }
  //   Serial.printf("\n");
  //   for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  //   {
  //     digitalWrite(ROWS_PINS[pin], 0);
  //   }
  // }
  // Serial.printf("\n");

  for (int i = 0; i < NUM_ROWS * NUM_COLS; i++)
  {
    strip.SetPixelColor(i, PREV_STATE[i] == 1 ? red : black);
  }
  strip.Show();

  delay(100);
}