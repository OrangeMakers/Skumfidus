#ifndef OMDISPLAY_H
#define OMDISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum class Alignment {
    LEFT,
    RIGHT
};

class OMDisplay {
public:
    OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void writeDisplay(const String& text, uint8_t row, uint8_t startCol, uint8_t endCol = 0, Alignment alignment = Alignment::LEFT);
    void writeAlert(const String& row1, const String& row2, unsigned long duration);
    void clearDisplay();
    static void updateTask(void* pvParameters);

private:
    LiquidCrystal_I2C _lcd;
    uint8_t _cols;
    uint8_t _rows;
    char _buffer[2][17];  // Assuming max 16 columns + null terminator
    enum class State {
        IDLE,
        UPDATING,
        ALERT
    };
    State _state;
    unsigned long _alertStartTime;
    unsigned long _alertDuration;

    void update();
};

#endif // OMDISPLAY_H
