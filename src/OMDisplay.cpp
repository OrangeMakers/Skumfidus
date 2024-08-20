#include "OMDisplay.h"

OMDisplay::OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows), _state(State::IDLE) {
    memset(_buffer, ' ', sizeof(_buffer));
    for (int i = 0; i < 2; i++) {
        _buffer[i][16] = '\0';
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

    _state = State::UPDATING;
}

void OMDisplay::writeAlert(const String& row1, const String& row2, unsigned long duration) {
    char newAlertBuffer[2][17];
    memset(newAlertBuffer[0], ' ', _cols);
    memset(newAlertBuffer[1], ' ', _cols);
    strncpy(newAlertBuffer[0], row1.c_str(), min(row1.length(), (unsigned int)_cols));
    strncpy(newAlertBuffer[1], row2.c_str(), min(row2.length(), (unsigned int)_cols));
    newAlertBuffer[0][_cols] = '\0';
    newAlertBuffer[1][_cols] = '\0';

    // Check if the new alert is different from the current one
    if (strcmp(newAlertBuffer[0], _currentAlertBuffer[0]) != 0 || 
        strcmp(newAlertBuffer[1], _currentAlertBuffer[1]) != 0) {
        
        // Update the alert buffers
        memcpy(_alertBuffer, newAlertBuffer, sizeof(_alertBuffer));
        memcpy(_currentAlertBuffer, newAlertBuffer, sizeof(_currentAlertBuffer));
        
        _alertStartTime = millis();
        _alertDuration = duration;
        _state = State::ALERT;
        _alertChanged = true;
    }
}

void OMDisplay::clearDisplay() {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _buffer[i][j] = ' ';
        }
        _buffer[i][_cols] = '\0';  // Ensure null termination
    }
    _state = State::UPDATING;
}

void OMDisplay::update() {
    switch (_state) {
        case State::UPDATING:
            for (int i = 0; i < _rows; i++) {
                _lcd.setCursor(0, i);
                _lcd.print(_buffer[i]);
            }
            _state = State::IDLE;
            break;
        case State::ALERT:
            if (_alertChanged) {
                for (int i = 0; i < _rows; i++) {
                    _lcd.setCursor(0, i);
                    _lcd.print(_alertBuffer[i]);
                }
                _alertChanged = false;
            }
            if (_alertDuration != 0 && millis() - _alertStartTime > _alertDuration) {
                _state = State::UPDATING;
            }
            break;
        default:
            break;
    }
}

void OMDisplay::updateTask(void* pvParameters) {
    OMDisplay* display = static_cast<OMDisplay*>(pvParameters);
    for (;;) {
        display->update();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
