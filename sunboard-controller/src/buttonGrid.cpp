#include "buttonGrid.h"
#include <Arduino.h>
#include "logging.h"

int ButtonGrid::getState()
{
    delay(1);
    return digitalRead(STATE_PIN);
}

void ButtonGrid::setRow(int row)
{
    TRACEF("setting row: %d: ", row + 1);
    for (int pin = 0; pin < MULTIPLEXER_INPUT_PINS_COUNT; pin++)
    {
        int val = ((row + 2) & (1 << pin)) >> pin;
        TRACEF("%d ", val);
        digitalWrite(ROW_MULTIPLEXER_INPUT_PINS[pin], val);
    }
    TRACEF("\n");
}

int ButtonGrid::setCol(int col)
{
    TRACEF("setting col: %d: ", col + 1);
    for (int pin = 0; pin < MULTIPLEXER_INPUT_PINS_COUNT; pin++)
    {
        int val = (col & (1 << pin)) >> pin;
        TRACEF("%d ", val);
        digitalWrite(COL_MULTIPLEXER_INPUT_PINS[pin], val);
    }
    TRACEF("\n");
    return getState();
}

void ButtonGrid::setup()
{
    for (int i = 0; i < MULTIPLEXER_INPUT_PINS_COUNT; i++)
    {
        pinMode(ROW_MULTIPLEXER_INPUT_PINS[i], OUTPUT);
        pinMode(COL_MULTIPLEXER_INPUT_PINS[i], OUTPUT);
    }
    pinMode(STATE_PIN, INPUT_PULLDOWN);
}

void ButtonGrid::parseButtonState()
{
    for (uint8_t row = 0; row < NUM_ROWS; row++)
    {
        setRow(row);
        for (uint8_t col = 0; col < NUM_COLS; col++)
        {
            int result = setCol(col);
            if (result)
            {
                TRACEF("%d %d pressed\n", row + 1, col + 1);
            }
            if (result != state[row][col])
            {
                setButton(row, col, result);
            }
        }
    }
}
