#include "MatrixDisplay.h"
#include <string.h>
#include <vector>

// Constructor: Initialize the LCD and buffers
MatrixDisplay::MatrixDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : _lcd(lcd_addr, lcd_cols, lcd_rows), _cols(lcd_cols), _rows(lcd_rows),
      _currentBuffer(_rows, std::vector<char>(_cols + 1, ' ')),
      _newBuffer(_rows, std::vector<char>(_cols + 1, ' ')) {
    // Initialize buffers with spaces and null-terminate
    for (int i = 0; i < _rows; i++) {
        _currentBuffer[i][_cols] = '\0';
        _newBuffer[i][_cols] = '\0';
    }
}

// Destructor is no longer needed as std::vector manages its own memory
MatrixDisplay::~MatrixDisplay() {}

// Initialize the LCD
void MatrixDisplay::begin() {
    _lcd.init();
    _lcd.backlight();
}

// Update the display content (thread-safe)
void MatrixDisplay::updateDisplay(const String& row1, const String& row2) {
    std::lock_guard<std::mutex> lock(_displayMutex);  // Lock for thread safety
    fillBuffer(_newBuffer, row1, row2);  // Fill the new buffer with content
    updateChangedCharacters();  // Update the LCD with changes
}

// Continuous update thread
void MatrixDisplay::updateThread() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(_displayMutex);  // Lock for thread safety
            updateChangedCharacters();  // Update the LCD with any changes
        }
        delay(50);  // Small delay to prevent too frequent updates
    }
}

// Update only the characters that have changed
void MatrixDisplay::updateChangedCharacters() {
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            if (_currentBuffer[row][col] != _newBuffer[row][col]) {
                _lcd.setCursor(col, row);  // Set cursor to the changed character
                _lcd.write(_newBuffer[row][col]);  // Write the new character
                _currentBuffer[row][col] = _newBuffer[row][col];  // Update current buffer
            }
        }
    }
}

// Fill the buffer with new content
void MatrixDisplay::fillBuffer(std::vector<std::vector<char>>& buffer, const String& row1, const String& row2) {
    const String* rows[2] = {&row1, &row2};
    for (int i = 0; i < _rows; i++) {
        const String& row = *rows[i];
        int len = row.length();
        for (int j = 0; j < _cols; j++) {
            buffer[i][j] = (j < len) ? row[j] : ' ';  // Fill with content or spaces
        }
        buffer[i][_cols] = '\0';  // Null-terminate the string
    }
}
