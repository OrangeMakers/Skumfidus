#include "MatrixDisplay.h"

MatrixDisplay::MatrixDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows),
      _buffer(_rows, std::vector<char>(_cols, ' ')),
      _updateNeeded(false), _updateTaskHandle(NULL), _bufferMutex(NULL) {
    _bufferMutex = xSemaphoreCreateMutex();
}

void MatrixDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

void MatrixDisplay::updateDisplay(const String& row1, const String& row2) {
    if (xSemaphoreTake(_bufferMutex, portMAX_DELAY) == pdTRUE) {
        fillBuffer(row1, row2);
        _updateNeeded = true;
        xSemaphoreGive(_bufferMutex);
    }
}

void MatrixDisplay::updateTask() {
    while (true) {
        if (_updateNeeded) {
            updateChangedCharacters();
            _updateNeeded = false;
        }
        vTaskDelay(pdMS_TO_TICKS(50));  // Check for updates every 50ms
    }
}

void MatrixDisplay::updateChangedCharacters() {
    if (xSemaphoreTake(_bufferMutex, portMAX_DELAY) == pdTRUE) {
        for (int row = 0; row < _rows; row++) {
            for (int col = 0; col < _cols; col++) {
                _lcd.setCursor(col, row);
                _lcd.write(_buffer[row][col]);
            }
        }
        xSemaphoreGive(_bufferMutex);
    }
}

void MatrixDisplay::fillBuffer(const String& row1, const String& row2) {
    const String rows[2] = {row1, row2};
    for (int i = 0; i < _rows; i++) {
        const String& row = rows[i];
        int len = std::min(static_cast<int>(row.length()), static_cast<int>(_cols));
        std::copy(row.begin(), row.begin() + len, _buffer[i].begin());
        std::fill(_buffer[i].begin() + len, _buffer[i].end(), ' ');
    }
}

void MatrixDisplay::startUpdateThread() {
    xTaskCreatePinnedToCore(
        updateTaskWrapper,
        "UpdateDisplay",
        2048,
        this,
        1,  // Lower priority
        &_updateTaskHandle,
        0   // Run on core 0
    );
}

void MatrixDisplay::stopUpdateThread() {
    if (_updateTaskHandle != NULL) {
        vTaskDelete(_updateTaskHandle);
        _updateTaskHandle = NULL;
    }
}

void MatrixDisplay::updateTaskWrapper(void* parameter) {
    static_cast<MatrixDisplay*>(parameter)->updateTask();
}
MatrixDisplay::~MatrixDisplay() {
    if (_bufferMutex != NULL) {
        vSemaphoreDelete(_bufferMutex);
    }
}
