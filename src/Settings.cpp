#include "Settings.h"
#include <EEPROM.h>

// Initialize EEPROM with 512 bytes
const int EEPROM_SIZE = 512;

extern ButtonHandler buttonRotarySwitch;

// Define speed constants
const float SPEED_MIN = 500.0f;
const float SPEED_MAX = 3500.0f;

Settings::Settings(MatrixDisplay& display, ESP32Encoder& encoder)
    : _display(display), _encoder(encoder), _isDone(false), _inEditMode(false), _currentMenuIndex(0), _lastEncoderValue(0),
      _totalSteps(0), _settingsChanged(false) {
    EEPROM.begin(EEPROM_SIZE);
    initializeMenuItems();
    loadSettingsFromEEPROM();
    _totalSteps = (_totalDistance / DISTANCE_PER_REV) * STEPS_PER_REV;
}

Settings::~Settings() {
    // Destructor implementation (if needed)
}

void Settings::loadSettingsFromEEPROM() {
    EEPROM.get(0, _cookTime);
    EEPROM.get(4, _totalDistance);
    EEPROM.get(8, _speed);

    // Validate loaded values and set defaults if necessary
    if (_cookTime < 5000 || _cookTime > 120000) _cookTime = 30000;
    if (_totalDistance < 50.0f || _totalDistance > 120.0f) _totalDistance = 50.0f;
    if (_speed < SPEED_MIN || _speed > SPEED_MAX) _speed = (SPEED_MIN + SPEED_MAX) / 2;

    // Recalculate _totalSteps
    _totalSteps = (_totalDistance / DISTANCE_PER_REV) * STEPS_PER_REV;

    _initialCookTime = _cookTime;
    _initialTotalDistance = _totalDistance;
    _initialSpeed = _speed;
    _settingsChanged = false;
    updateMenuVisibility();
}

void Settings::saveSettingsToEEPROM() {
    EEPROM.put(0, _cookTime);
    EEPROM.put(4, _totalDistance);
    EEPROM.put(8, _speed);
    EEPROM.commit();

    _initialCookTime = _cookTime;
    _initialTotalDistance = _totalDistance;
    _initialSpeed = _speed;
    _settingsChanged = false;
    updateMenuVisibility();
}

unsigned long Settings::getCookTime() const { return _cookTime; }
float Settings::getTotalDistance() const { return _totalDistance; }
float Settings::getSpeed() const { return _speed; }
int Settings::getTotalSteps() const { return _totalSteps; }

void Settings::factoryReset() {
    _cookTime = 30000;
    _totalDistance = 50.0f;
    _speed = (SPEED_MIN + SPEED_MAX) / 2;
    saveSettingsToEEPROM();
    updateDisplay();
    _settingsChanged = false;
    updateMenuVisibility();
}

void Settings::enter() {
    _isDone = false;
    _inEditMode = false;
    _currentMenuIndex = 0;  // Always start at the first menu item (COOK_TIME)
    _lastEncoderValue = _encoder.getCount();
    updateMenuVisibility();
    _display.updateDisplay("", "");  // Clear the display
    displayCurrentMenuItem();
}

void Settings::exit() {
    _isDone = true;
    _inEditMode = false;
    _display.updateDisplay("", "");  // Clear the display
}

void Settings::update() {
    int8_t direction = getEncoderDirection();
    if (direction != 0) {
        if (_inEditMode) {
            adjustValue(direction);
        } else {
            handleMenuNavigation(direction);
        }
    }
    
    if (buttonRotarySwitch.isPressed()) {
        if (_inEditMode) {
            exitEditMode();
        } else {
            handleMenuSelection();
        }
    }
}

void Settings::handleMenuNavigation(int8_t direction) {
    do {
        if (direction > 0) {
            _currentMenuIndex = (_currentMenuIndex + 1) % _menuItems.size();
        } else {
            _currentMenuIndex = (_currentMenuIndex > 0) ? _currentMenuIndex - 1 : _menuItems.size() - 1;
        }
    } while (!_menuItems[_currentMenuIndex].visible);
    displayCurrentMenuItem();
}

void Settings::handleMenuSelection() {
    switch (_menuItems[_currentMenuIndex].item) {
        case MenuItem::COOK_TIME:
        case MenuItem::TOTAL_DISTANCE:
        case MenuItem::MAX_SPEED:
            enterEditMode();
            break;
        case MenuItem::LOAD_EEPROM:
            loadSettingsFromEEPROM();
            _display.updateDisplay("Settings Loaded", "");
            delay(1000);
            break;
        case MenuItem::SAVE_EEPROM:
            if (confirmAction("Save Settings?")) {
                saveSettingsToEEPROM();
                _display.updateDisplay("Settings Saved", "");
                delay(1000);
            }
            break;
        case MenuItem::EXIT:
            _isDone = true;
            break;
        case MenuItem::FACTORY_RESET:
            if (confirmAction("Factory Reset?")) {
                factoryReset();
                _display.updateDisplay("Factory Reset", "Complete");
                delay(2000);
            }
            break;
    }
    updateMenuVisibility();
    displayCurrentMenuItem();
}

bool Settings::confirmAction(const char* message) {
    _display.updateDisplay(message, "Yes");
    
    bool confirmed = true;
    int32_t lastConfirmEncoderValue = _encoder.getCount();
    
    while (!buttonRotarySwitch.isPressed()) {
        int32_t newEncoderValue = _encoder.getCount();
        if (newEncoderValue != lastConfirmEncoderValue) {
            confirmed = !confirmed;
            _display.updateDisplay(message, confirmed ? "Yes" : "No");
            lastConfirmEncoderValue = newEncoderValue;
        }
        buttonRotarySwitch.update();
    }
    
    return confirmed;
}

bool Settings::isDone() const {
    return _isDone;
}

void Settings::initializeMenuItems() {
    _menuItems = {
        {MenuItem::COOK_TIME, "Cook Time", true},
        {MenuItem::TOTAL_DISTANCE, "Total Distance", true},
        {MenuItem::MAX_SPEED, "Max Speed", true},
        {MenuItem::LOAD_EEPROM, "Load EEPROM", false},
        {MenuItem::SAVE_EEPROM, "Save EEPROM", false},
        {MenuItem::EXIT, "Exit", true},
        {MenuItem::FACTORY_RESET, "Factory Reset", false}
    };
}

void Settings::updateMenuVisibility() {
    _settingsChanged = (_cookTime != _initialCookTime) ||
                       (_totalDistance != _initialTotalDistance) ||
                       (_speed != _initialSpeed);

    for (auto& item : _menuItems) {
        switch (item.item) {
            case MenuItem::LOAD_EEPROM:
            case MenuItem::SAVE_EEPROM:
            case MenuItem::FACTORY_RESET:
                item.visible = _settingsChanged;
                break;
            default:
                item.visible = true;
                break;
        }
    }
}

void Settings::displayCurrentMenuItem() {
    String topLine = _menuItems[_currentMenuIndex].displayName;
    String bottomLine;
    
    switch (_menuItems[_currentMenuIndex].item) {
        case MenuItem::COOK_TIME:
            bottomLine = String(_cookTime / 1000) + "s";
            break;
        case MenuItem::TOTAL_DISTANCE:
            bottomLine = String(_totalDistance, 1) + "mm";
            break;
        case MenuItem::MAX_SPEED:
            bottomLine = String(map(_speed, SPEED_MIN, SPEED_MAX, 0, 100)) + "%";
            break;
        default:
            break;
    }
    
    _display.updateDisplay(topLine, bottomLine);
}

int8_t Settings::getEncoderDirection() {
    int32_t newValue = _encoder.getCount();
    int8_t direction = 0;
    if (newValue > _lastEncoderValue) {
        direction = 1;
    } else if (newValue < _lastEncoderValue) {
        direction = -1;
    }
    _lastEncoderValue = newValue;
    return direction;
}

void Settings::enterEditMode() {
    _inEditMode = true;
    displayCurrentMenuItem();
}

void Settings::exitEditMode() {
    _inEditMode = false;
    displayCurrentMenuItem();
}

void Settings::adjustValue(int8_t direction) {
    switch (_menuItems[_currentMenuIndex].item) {
        case MenuItem::COOK_TIME:
            adjustCookTime(direction);
            break;
        case MenuItem::TOTAL_DISTANCE:
            adjustTotalDistance(direction);
            break;
        case MenuItem::MAX_SPEED:
            adjustMaxSpeed(direction);
            break;
        default:
            break;
    }
    updateDisplay();
}

void Settings::adjustCookTime(int8_t direction) {
    _cookTime += direction * 1000; // Adjust by 1 second
    if (_cookTime < 5000) _cookTime = 5000; // Minimum 5 seconds
    if (_cookTime > 120000) _cookTime = 120000; // Maximum 120 seconds
}

void Settings::adjustTotalDistance(int8_t direction) {
    _totalDistance += direction * 5.0f; // Adjust by 5mm
    if (_totalDistance < 50.0f) _totalDistance = 50.0f; // Minimum 50mm
    if (_totalDistance > 120.0f) _totalDistance = 120.0f; // Maximum 120mm
    
    // Recalculate TOTAL_STEPS
    _totalSteps = (_totalDistance / DISTANCE_PER_REV) * STEPS_PER_REV;
}

void Settings::adjustMaxSpeed(int8_t direction) {
    float step = (SPEED_MAX - SPEED_MIN) / 100.0f; // Adjust by 1% of the speed range
    _speed += direction * step;
    _speed = constrain(_speed, SPEED_MIN, SPEED_MAX);
}

void Settings::updateDisplay() {
    String value;
    switch (_menuItems[_currentMenuIndex].item) {
        case MenuItem::COOK_TIME:
            value = String(_cookTime / 1000) + "s";
            break;
        case MenuItem::TOTAL_DISTANCE:
            value = String(_totalDistance, 1) + "mm";
            break;
        case MenuItem::MAX_SPEED:
            value = String(((_speed - SPEED_MIN) / (SPEED_MAX - SPEED_MIN) * 100), 0) + "%";
            break;
        default:
            value = "";
            break;
    }
    _display.updateDisplay(_menuItems[_currentMenuIndex].displayName, value);
}
