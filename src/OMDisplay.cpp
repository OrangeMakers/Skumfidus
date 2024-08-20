#include "OMDisplay.h"

OMDisplay::OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows), _updateNeeded(false), _alertActive(false) {
    memset(_buffer, ' ', sizeof(_buffer));
    memset(_alertBuffer, ' ', sizeof(_alertBuffer));
    memset(_currentAlertBuffer, ' ', sizeof(_currentAlertBuffer));
    for (int i = 0; i < 2; i++) {
        _buffer[i][16] = '\0';
        _alertBuffer[i][16] = '\0';
        _currentAlertBuffer[i][16] = '\0';
    }
}

void OMDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

void OMDisplay::writeDisplay(const String& text, uint8_t row, uint8_t startCol, uint8_t endCol, Alignment alignment) {
    if (row >= _rows || startCol >= _cols) return;
    
    endCol = (endCol == 0 || endCol > _cols) ? _cols : endCol;
    int len = text.length();
    int availableSpace = endCol - startCol;

    if (alignment == Alignment::RIGHT && len < availableSpace) {
        startCol = endCol - len;
    }

    for (int i = 0; i < availableSpace; i++) {
        if (i < len) {
            _buffer[row][startCol + i] = text[i];
        } else {
            _buffer[row][startCol + i] = ' ';
        }
    }

    _updateNeeded = true;
    _alertActive = false;
}

void OMDisplay::writeAlert(const String& row1, const String& row2, unsigned long duration) {
    memset(_alertBuffer[0], ' ', _cols);
    memset(_alertBuffer[1], ' ', _cols);
    strncpy(_alertBuffer[0], row1.c_str(), min(row1.length(), (unsigned int)_cols));
    strncpy(_alertBuffer[1], row2.c_str(), min(row2.length(), (unsigned int)_cols));
    _alertBuffer[0][_cols] = '\0';
    _alertBuffer[1][_cols] = '\0';

    _alertStartTime = millis();
    _alertDuration = duration;
    _updateNeeded = true;
    _alertActive = true;
}

void OMDisplay::clearDisplay() {
    for (int i = 0; i < _rows; i++) {
        memset(_buffer[i], ' ', _cols);
        _buffer[i][_cols] = '\0';  // Ensure null termination
    }
    _updateNeeded = true;
    _alertActive = false;
}

void OMDisplay::update() {
    if (_updateNeeded) {
        if (_alertActive) {
            for (int i = 0; i < _rows; i++) {
                _lcd.setCursor(0, i);
                _lcd.print(_alertBuffer[i]);
            }
            if (_alertDuration != 0 && millis() - _alertStartTime > _alertDuration) {
                _alertActive = false;
            }
        } else {
            for (int i = 0; i < _rows; i++) {
                _lcd.setCursor(0, i);
                _lcd.print(_buffer[i]);
            }
        }
        _updateNeeded = false;
    }
}

void OMDisplay::updateTask(void* pvParameters) {
    OMDisplay* display = static_cast<OMDisplay*>(pvParameters);
    for (;;) {
        display->update();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
