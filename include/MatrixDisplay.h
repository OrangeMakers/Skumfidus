#ifndef MATRIX_DISPLAY_H
#define MATRIX_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

class MatrixDisplay {
public:
    MatrixDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    ~MatrixDisplay();
    void begin();
    void updateDisplay(const String& row1, const String& row2);
    void startUpdateThread();
    void stopUpdateThread();

private:
    LiquidCrystal_I2C _lcd;
    uint8_t _cols;
    uint8_t _rows;
    std::vector<std::vector<char>> _buffer;
    volatile bool _updateNeeded;
    TaskHandle_t _updateTaskHandle;
    SemaphoreHandle_t _bufferMutex;

    static void updateTaskWrapper(void* parameter);
    void updateTask();
    void updateChangedCharacters();
    void fillBuffer(const String& row1, const String& row2);
};

#endif // MATRIX_DISPLAY_H
