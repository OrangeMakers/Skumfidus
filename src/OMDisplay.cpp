#include "OMDisplay.h"

OMDisplay::OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows), 
      _state(DisplayState::IDLE), _updateNeeded(false) {
    memset(_currentBuffer, ' ', sizeof(_currentBuffer));
    memset(_newBuffer, ' ', sizeof(_newBuffer));
    memset(_bufferedMessage, ' ', sizeof(_bufferedMessage));
    for (int i = 0; i < 2; i++) {
        _currentBuffer[i][16] = '\0';
        _newBuffer[i][16] = '\0';
        _bufferedMessage[i][16] = '\0';
    }
}

void OMDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

void OMDisplay::writeDisplay(const String& row1, const String& row2, unsigned long duration) {
    fillBuffer(_newBuffer, row1, row2);

    if (!compareBuffers(_currentBuffer, _newBuffer)) {
        if (duration > 0) {
            copyBuffer(_bufferedMessage, _newBuffer);
            _state = DisplayState::DISPLAYING;
            _displayStartTime = millis();
            _displayDuration = duration;
        } else {
            copyBuffer(_currentBuffer, _newBuffer);
            _updateNeeded = true;
            _state = DisplayState::IDLE;
        }
    }
}

void OMDisplay::writeDisplayNoQueue(const String& row1, const String& row2, unsigned long duration) {
    fillBuffer(_newBuffer, row1, row2);

    if (!compareBuffers(_currentBuffer, _newBuffer)) {
        copyBuffer(_currentBuffer, _newBuffer);
        _updateNeeded = true;
        _state = DisplayState::IDLE;

        if (duration > 0) {
            _displayStartTime = millis();
            _displayDuration = duration;
            _state = DisplayState::DISPLAYING;
        }
    }
}

void OMDisplay::update() {
    switch (_state) {
        case DisplayState::IDLE:
            if (_updateNeeded) {
                for (int i = 0; i < _rows; i++) {
                    _lcd.setCursor(0, i);
                    _lcd.print(_currentBuffer[i]);
                }
                _updateNeeded = false;
            }
            break;

        case DisplayState::DISPLAYING:
            if (_updateNeeded || millis() - _displayStartTime < _displayDuration) {
                for (int i = 0; i < _rows; i++) {
                    _lcd.setCursor(0, i);
                    _lcd.print(_bufferedMessage[i]);
                }
                _updateNeeded = false;
            } else if (millis() - _displayStartTime >= _displayDuration) {
                copyBuffer(_currentBuffer, _bufferedMessage);
                _updateNeeded = true;
                _state = DisplayState::IDLE;
            }
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

void OMDisplay::fillBuffer(char buffer[2][17], const String& row1, const String& row2) {
    strncpy(buffer[0], row1.c_str(), _cols);
    strncpy(buffer[1], row2.c_str(), _cols);
    for (int i = 0; i < 2; i++) {
        for (int j = strlen(buffer[i]); j < _cols; j++) {
            buffer[i][j] = ' ';
        }
        buffer[i][_cols] = '\0';
    }
}

bool OMDisplay::compareBuffers(const char buffer1[2][17], const char buffer2[2][17]) {
    return (strcmp(buffer1[0], buffer2[0]) == 0) && (strcmp(buffer1[1], buffer2[1]) == 0);
}

void OMDisplay::copyBuffer(char dest[2][17], const char src[2][17]) {
    memcpy(dest, src, sizeof(_currentBuffer));
}
