#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t pin, bool activeLow)
    : _pin(pin), _activeLow(activeLow), _lastState(false), _currentState(false), _changed(false), _lastDebounceTime(0)
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
