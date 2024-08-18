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
    writeDisplay(row1, 0, 0);
    writeDisplay(row2, 1, 0);
    _alertStartTime = millis();
    _alertDuration = duration;
    _state = State::ALERT;
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
            if (millis() - _alertStartTime > _alertDuration) {
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
