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
  // strip.Begin();
  // strip.Show();
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

// set the pins on the multiplexer for the given row. Since the PCB is connected from pin 2 to 15, this has to be added by 2.
// so for example, if i want to activate the 0th row, i have to send "2" to the multiplexer.
void setRow(uint8_t row)
{
  Serial.printf("setting row: %d: ", row);
  row = row - 1;
  for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  {
    int val = ((row + 2) & (1 << pin)) >> pin;
    Serial.printf("%d ", val);
    digitalWrite(ROWS_PINS[pin], val);
  }
  Serial.printf("\n");
}

void setColumn(uint8_t col)
{
  Serial.printf("setting col: %d: ", col);
  col = col - 1;
  for (int pin = 0; pin < NUM_BUTT_INPUT_PINS; pin++)
  {
    int val = (col & (1 << pin)) >> pin;
    Serial.printf("%d ", val);
    digitalWrite(COLS_PINS[pin], val);
  }
  Serial.printf("\n");
}

void loop()
{
  setRow(1);
  delay(5000);
  setRow(2);
  delay(5000);
  setRow(3);
  delay(5000);
  setRow(4);
  delay(5000);
  setRow(5);
  delay(5000);
  setRow(6);
  delay(5000);
  setRow(7);
  delay(5000);
  setRow(8);
  delay(5000);
  setRow(9);
  delay(5000);
  setRow(10);
  delay(5000);
  setRow(11);
  delay(5000);
  setRow(12);
  delay(5000);
  setRow(13);
  delay(5000);
  setRow(14);
  delay(5000);

  // go through the number of rows
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

  // for (int i = 0; i < NUM_ROWS * NUM_COLS; i++)
  // {
  //   strip.SetPixelColor(i, PREV_STATE[i] == 1 ? red : black);
  // }
  // strip.Show();
}