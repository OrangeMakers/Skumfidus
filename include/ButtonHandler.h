#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
    ButtonHandler(uint8_t pin, bool activeLow = true);
    void begin();
    void update();
    bool isPressed();
    bool isReleased();
    bool stateChanged();
    bool getState();
    void reset();

private:
    uint8_t _pin;
    bool _activeLow;
    bool _lastState;
    bool _currentState;
    bool _changed;
    unsigned long _lastDebounceTime;
    static const unsigned long DEBOUNCE_DELAY = 50;  // 50ms debounce time
};

#endif // BUTTON_HANDLER_H
