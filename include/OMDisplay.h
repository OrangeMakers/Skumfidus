#ifndef OMDISPLAY_H
#define OMDISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class OMDisplay {
public:
    OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void writeDisplay(const String& row1, const String& row2, unsigned long duration = 0);
    void writeDisplayNoQueue(const String& row1, const String& row2, unsigned long duration = 0);
    static void updateTask(void* pvParameters);
    void update();

private:
    struct DisplayMessage {
        char content[2][17];
        unsigned long duration;
    };

    LiquidCrystal_I2C _lcd;
    uint8_t _cols;
    uint8_t _rows;
    char _currentBuffer[2][17];  // Current display content
    DisplayMessage _activeMessage;
    DisplayMessage _queuedMessage;
    unsigned long _displayStartTime;
    bool _updateNeeded;
    bool _messageActive;

    void fillBuffer(char buffer[2][17], const String& row1, const String& row2);
    bool compareBuffers(const char buffer1[2][17], const char buffer2[2][17]);
    void copyBuffer(char dest[2][17], const char src[2][17]);
    void setActiveMessage(const char content[2][17], unsigned long duration);
    void setQueuedMessage(const char content[2][17], unsigned long duration);
    bool isQueuedMessagePresent() const;
    void clearQueuedMessage();
};

#endif // OMDISPLAY_H
