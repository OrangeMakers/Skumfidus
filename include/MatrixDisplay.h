#ifndef MATRIX_DISPLAY_H
#define MATRIX_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <mutex>
#include <atomic>
#include <vector>

class MatrixDisplay {
public:
    MatrixDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    ~MatrixDisplay();
    void begin();
    void updateDisplay(const String& row1, const String& row2);

private:
    LiquidCrystal_I2C _lcd;
    std::mutex _displayMutex;
    uint8_t _cols;
    uint8_t _rows;
    std::vector<std::vector<char>> _currentBuffer;
    std::vector<std::vector<char>> _newBuffer;

    void updateThread();
    void updateChangedCharacters();
    void fillBuffer(std::vector<std::vector<char>>& buffer, const String& row1, const String& row2);
};

#endif // MATRIX_DISPLAY_H
