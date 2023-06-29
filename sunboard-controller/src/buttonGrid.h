#ifndef ButtonGrid_h
#define ButtonGrid_h

const int NUM_ROWS = 14;
const int NUM_COLS = 11;

const int MULTIPLEXER_INPUT_PINS_COUNT = 4;
const int ROW_MULTIPLEXER_INPUT_PINS[MULTIPLEXER_INPUT_PINS_COUNT] = {17, 16, 27, 13};
const int COL_MULTIPLEXER_INPUT_PINS[MULTIPLEXER_INPUT_PINS_COUNT] = {22, 21, 26, 25};
const int STATE_PIN = 23;

class ButtonGrid
{
private:
    int state[NUM_ROWS][NUM_COLS] = {0};
    int getState();
    void setRow(int row);
    int setCol(int col);

public:
    void setup();
    void setButton(int row, int column, int value);
    void parseButtonState();
    int getButton(int row, int col);
};

#endif