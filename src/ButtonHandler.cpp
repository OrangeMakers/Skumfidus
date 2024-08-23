#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t pin, const char* name, bool activeLow)
    : _pin(pin), _name(name), _activeLow(activeLow), _lastState(false), _currentState(false), _changed(false), _lastDebounceTime(0), _pressStartTime(0)
{}

void ButtonHandler::begin() {
    pinMode(_pin, _activeLow ? INPUT_PULLUP : INPUT);
}

void ButtonHandler::update() {
    bool reading = digitalRead(_pin);
    if (_activeLow) reading = !reading;

    if (reading != _lastState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != _currentState) {
            _currentState = reading;
            _changed = true;
            if (_currentState) {
                _pressStartTime = millis();
            }
            #ifdef DEBUG
            Serial.print(_name);
            Serial.print(" button ");
            Serial.println(_currentState ? "pressed" : "released");
            #endif
        }
    }

    _lastState = reading;
}

bool ButtonHandler::isPressed() {
    return _currentState && _changed;
}

bool ButtonHandler::isReleased() {
    return !_currentState && _changed;
}

bool ButtonHandler::stateChanged() {
    return _changed;
}

bool ButtonHandler::getState() {
    return _currentState;
}

void ButtonHandler::reset() {
    _changed = false;
}

unsigned long ButtonHandler::isPressedForMs() {
    if (_currentState) {
        return millis() - _pressStartTime;
    }
    return 0;
}
