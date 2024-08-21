#include "MatrixDisplay.h"
#include <queue>

MatrixDisplay::MatrixDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows),
      _buffer(_rows, std::vector<char>(_cols, ' ')),
      _updateNeeded(false), _updateTaskHandle(NULL), _bufferMutex(NULL),
      _messageQueueMutex(NULL), _currentMessageEndTime(0) {
    _bufferMutex = xSemaphoreCreateMutex();
    _messageQueueMutex = xSemaphoreCreateMutex();
}

void MatrixDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

void MatrixDisplay::updateDisplay(const String& row1, const String& row2, unsigned long displayDuration) {
    if (xSemaphoreTake(_messageQueueMutex, portMAX_DELAY) == pdTRUE) {
        unsigned long currentTime = millis();
        DisplayMessage newMessage = {row1, row2, currentTime + displayDuration};

        if (displayDuration > 0) {
            if (_messageQueue.empty() || currentTime >= _currentMessageEndTime) {
                // If queue is empty or current message has expired, display immediately
                _messageQueue = std::queue<DisplayMessage>();  // Clear the queue
                _messageQueue.push(newMessage);
                _currentMessageEndTime = newMessage.endTime;
                updateBufferWithMessage(newMessage);
            } else {
                // Add to queue, replacing any existing queued message
                if (_messageQueue.size() > 1) {
                    _messageQueue.pop();  // Remove the old queued message
                }
                _messageQueue.push(newMessage);
            }
        } else {
            // For immediate display (duration = 0), clear queue and display
            _messageQueue = std::queue<DisplayMessage>();
            updateBufferWithMessage(newMessage);
        }

        xSemaphoreGive(_messageQueueMutex);
    }
}

void MatrixDisplay::updateBufferWithMessage(const DisplayMessage& message) {
    if (xSemaphoreTake(_bufferMutex, portMAX_DELAY) == pdTRUE) {
        fillBuffer(message.row1, message.row2);
        _updateNeeded = true;
        xSemaphoreGive(_bufferMutex);
    }
}

void MatrixDisplay::updateTask() {
    while (true) {
        unsigned long currentTime = millis();

        if (xSemaphoreTake(_messageQueueMutex, portMAX_DELAY) == pdTRUE) {
            if (!_messageQueue.empty() && currentTime >= _currentMessageEndTime) {
                DisplayMessage nextMessage = _messageQueue.front();
                _messageQueue.pop();
                _currentMessageEndTime = nextMessage.endTime;
                updateBufferWithMessage(nextMessage);
            }
            xSemaphoreGive(_messageQueueMutex);
        }

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
    if (_messageQueueMutex != NULL) {
        vSemaphoreDelete(_messageQueueMutex);
    }
}
