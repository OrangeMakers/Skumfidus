# Settings System Implementation

## Overview
The Settings system allows users to update parameters stored in EEPROM. It is accessible only from the IDLE state by pressing the Rotary switch.

## Menu Structure
1. Cook Time
2. Total Distance
3. Max Speed
4. Load EEPROM
5. Save EEPROM
6. Exit
7. Factory Reset

## Navigation
- Use the rotary encoder to scroll through menu items.
- Press the rotary switch to select an item or enter/exit edit mode.
- The ESP32Encoder library is used for reading the rotary encoder.

## Encoder Implementation
- Initialize the encoder in the `Settings` class constructor:
  ```cpp
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachHalfQuad(ROTARY_CLK_PIN, ROTARY_DT_PIN);
  encoder.setCount(0);
  ```
- Read encoder value in the update method:
  ```cpp
  int32_t newValue = encoder.getCount();
  int32_t encoderDelta = newValue - lastEncoderValue;
  lastEncoderValue = newValue;
  ```
- Use `encoderDelta` to navigate menu items or adjust values.

## Detailed Menu Items

### 1. Cook Time
- Display: "Cook Time: XXs" (XXs is the current value in seconds)
- Editable range: 5 to 120 seconds
- Default value: 30 seconds
- Increment: 1 second per rotary encoder click

### 2. Total Distance
- Display: "Total Dist: XX mm" (XX is the current value in mm)
- Editable range: 50 mm to 120 mm
- Default value: 50 mm
- Increment: 5 mm per rotary encoder click

### 3. Max Speed
- Display: "Max Speed: XX %" (XX is the current value in percentage)
- Editable range: 33 % to 100 % (corresponding to 800-2400 steps/sec)
- Default value: 50 %
- Increment: 1 % per rotary encoder click

### 4. Load EEPROM
- Only visible if current settings differ from EEPROM values
- Display: "Load EEPROM"
- Action: Load saved values from EEPROM

### 5. Save EEPROM
- Only visible if current settings differ from EEPROM values
- Display: "Save EEPROM"
- Action: Save current values to EEPROM

### 6. Exit
- Always visible
- Display: "Exit"
- Action: Return to IDLE state

### 7. Factory Reset
- Only visible if any value differs from factory defaults
- Display: "Factory Reset"
- Action: Reset all values to factory defaults

## Interaction Flow

1. Enter Settings:
   - From IDLE state, press and hold the rotary switch for 1 second
   - Display shows "Settings Menu", then shows the first menu item

2. Navigate Menu:
   - Rotate encoder clockwise: Move to next menu item
   - Rotate encoder counter-clockwise: Move to previous menu item
   - Display shows current menu item

3. Select Menu Item:
   - Press rotary switch to select the current menu item

4. Edit Value (for Cook Time, Total Distance, Max Speed):
   - Display inverts colors (white background, black text) to indicate edit mode
   - Display shows current value
   - Rotate encoder to change value
   - Press rotary switch to confirm and exit edit mode, returning to normal display colors

5. Load EEPROM:
   - Press rotary switch to select
   - Display shows "Loading..." for 1 second
   - Then shows "Values Loaded" for 1 second
   - Return to Cook Time menu item

6. Save EEPROM:
   - Press rotary switch to select
   - Display shows "Saving..." for 1 second
   - Then shows "Values Saved" for 1 second
   - Exit to IDLE state

7. Exit:
   - Press rotary switch to select
   - If values are changed but not saved, display "Discard changes?" for 2 seconds
     - Rotate encoder to choose "Yes" or "No"
     - Press rotary switch to confirm
   - If "Yes" or no changes made, return to IDLE state

8. Factory Reset:
   - Press rotary switch to select
   - Display shows "Confirm Reset?" for 2 seconds
   - Rotate encoder to choose "Yes" or "No"
   - Press rotary switch to confirm
   - If "Yes", reset values, save to EEPROM, and return to IDLE state

## State Tracking
- Maintain a `bool` flag for each editable value to track if it has been changed
- Use these flags to determine visibility of Load/Save EEPROM and Factory Reset options
- Reset flags when saving to EEPROM or performing a factory reset

## Recalculation
- After changing Total Distance, recalculate TOTAL_STEPS
- Formula: TOTAL_STEPS = (TOTAL_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV

## Implementation Details
- Create a new `Settings` class in a separate file (e.g., `Settings.h` and `Settings.cpp`)
- Implement a state machine for the settings menu in this class
- Use direct LCD control for display updates in the Settings menu
- Maintain consistent naming conventions (e.g., camelCase for methods, UPPER_CASE for constants)

## Display Handling
- Disable OMDisplay updates while in the Settings menu
- Add methods to OMDisplay class to enable/disable updates:
  ```cpp
  void OMDisplay::setUpdateEnabled(bool enabled) {
      _updateEnabled = enabled;
  }
  ```
- In the Settings class, directly control the LCD using LiquidCrystal_I2C
- Pass a reference to the LCD object to the Settings class constructor

## Integration
- In `main.cpp`, create an instance of the `Settings` class
- In the IDLE state handler, check for rotary switch press to enter settings menu
- When entering Settings menu:
  ```cpp
  void enterSettingsMenu() {
      display.setUpdateEnabled(false);  // Disable OMDisplay updates
      settings.enter();  // Enter settings menu
  }
  ```
- When exiting Settings menu:
  ```cpp
  void exitSettingsMenu() {
      settings.exit();  // Exit settings menu
      display.setUpdateEnabled(true);  // Re-enable OMDisplay updates
  }
  ```
- In the main loop, handle the Settings menu state:
  ```cpp
  case SETTINGS_MENU:
      settings.update();
      if (settings.isDone()) {
          exitSettingsMenu();
          changeState(IDLE);
      }
      break;
  ```
- When in settings menu, delegate all control to the `Settings` class until it signals a return to IDLE state
