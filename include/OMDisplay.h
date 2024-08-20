#ifndef OMDISPLAY_H
#define OMDISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class OMDisplay {
public:
    OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void writeDisplay(const String& row1, const String& row2, unsigned long duration = 0);
    static void updateTask(void* pvParameters);
    void update();

private:
    enum class DisplayState {
        IDLE,
        DISPLAYING,
        BUFFERED
    };

    LiquidCrystal_I2C _lcd;
    uint8_t _cols;
    uint8_t _rows;
    char _currentBuffer[2][17];  // Current display content
    char _newBuffer[2][17];      // New content to be displayed
    char _bufferedMessage[2][17]; // Buffered message
    DisplayState _state;
    unsigned long _displayStartTime;
    unsigned long _displayDuration;
    bool _updateNeeded;

    void fillBuffer(char buffer[2][17], const String& row1, const String& row2);
    bool compareBuffers(const char buffer1[2][17], const char buffer2[2][17]);
    void copyBuffer(char dest[2][17], const char src[2][17]);
};

#endif // OMDISPLAY_H
