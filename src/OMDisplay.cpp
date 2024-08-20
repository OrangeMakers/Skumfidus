#include "OMDisplay.h"

OMDisplay::OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows), 
      _updateNeeded(false), _messageActive(false) {
    memset(_currentBuffer, ' ', sizeof(_currentBuffer));
    memset(_activeMessage.content, ' ', sizeof(_activeMessage.content));
    memset(_queuedMessage.content, ' ', sizeof(_queuedMessage.content));
    for (int i = 0; i < 2; i++) {
        _currentBuffer[i][16] = '\0';
        _activeMessage.content[i][16] = '\0';
        _queuedMessage.content[i][16] = '\0';
    }
    _activeMessage.duration = 0;
    _queuedMessage.duration = 0;
}

void OMDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

void OMDisplay::writeDisplay(const String& row1, const String& row2, unsigned long duration) {
    char newBuffer[2][17];
    fillBuffer(newBuffer, row1, row2);

    if (_messageActive) {
        setQueuedMessage(newBuffer, duration);
    } else {
        setActiveMessage(newBuffer, duration);
    }
}

void OMDisplay::writeDisplayNoQueue(const String& row1, const String& row2, unsigned long duration) {
    if (!_messageActive) {
        char newBuffer[2][17];
        fillBuffer(newBuffer, row1, row2);
        copyBuffer(_currentBuffer, newBuffer);
        _updateNeeded = true;
    }
}

void OMDisplay::update() {
    unsigned long currentTime = millis();

    if (_messageActive) {
        if (currentTime - _displayStartTime >= _activeMessage.duration) {
            _messageActive = false;
            if (_queuedMessage.duration > 0) {
                setActiveMessage(_queuedMessage.content, _queuedMessage.duration);
                memset(_queuedMessage.content, ' ', sizeof(_queuedMessage.content));
                _queuedMessage.duration = 0;
            } else {
                copyBuffer(_currentBuffer, _activeMessage.content);
                _updateNeeded = true;
            }
        } else {
            copyBuffer(_currentBuffer, _activeMessage.content);
            _updateNeeded = true;
        }
    }

    if (_updateNeeded) {
        for (int i = 0; i < _rows; i++) {
            _lcd.setCursor(0, i);
            _lcd.print(_currentBuffer[i]);
        }
        _updateNeeded = false;
    }
}

void OMDisplay::setActiveMessage(const char content[2][17], unsigned long duration) {
    copyBuffer(_activeMessage.content, content);
    _activeMessage.duration = duration;
    _messageActive = true;
    _displayStartTime = millis();
    _updateNeeded = true;
}

void OMDisplay::setQueuedMessage(const char content[2][17], unsigned long duration) {
    copyBuffer(_queuedMessage.content, content);
    _queuedMessage.duration = duration;
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
