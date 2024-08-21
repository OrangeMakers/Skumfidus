# Configuration Guide

This guide provides instructions on how to configure the Marsh Mellow 2.0 system, including adjustable parameters and their effects on system behavior.

## Configurable Parameters

The following parameters can be adjusted through the Settings menu:

1. Cook Time
2. Total Distance
3. Max Speed

Additionally, there are options to load from EEPROM, save to EEPROM, and perform a factory reset.

## Accessing the Settings Menu

1. Ensure the system is in the IDLE state.
2. Press and hold the rotary encoder button for 1 second.
3. The LCD will display "Settings Menu" briefly, then show the first menu item.

## Navigating the Settings Menu

- Rotate the encoder clockwise to move to the next menu item.
- Rotate counterclockwise to move to the previous menu item.
- Press the rotary encoder button to select a menu item or enter/exit edit mode.

## Adjustable Parameters

### 1. Cook Time

- Display: "Cook Time: XXs" (XX is the current value in seconds)
- Editable range: 5 to 120 seconds
- Default value: 30 seconds
- Increment: 1 second per encoder click
- Effect: Determines the duration of each cooking cycle

### 2. Total Distance

- Display: "Total Dist: XX mm" (XX is the current value in mm)
- Editable range: 50 mm to 120 mm
- Default value: 50 mm
- Increment: 5 mm per encoder click
- Effect: Sets the total travel distance of the stepper motor in each direction

### 3. Max Speed

- Display: "Max Speed: XX %" (XX is the current value in percentage)
- Editable range: 33% to 100% (corresponding to 800-2400 steps/sec)
- Default value: 50%
- Increment: 1% per encoder click
- Effect: Determines the maximum speed of the stepper motor

## Additional Menu Options

### 4. Load EEPROM

- Only visible if current settings differ from EEPROM values
- Action: Loads saved values from EEPROM, overwriting current settings

### 5. Save EEPROM

- Only visible if current settings differ from EEPROM values
- Action: Saves current values to EEPROM for persistence across power cycles

### 6. Exit

- Action: Returns to the IDLE state
- If settings have changed but not saved, prompts for confirmation

### 7. Factory Reset

- Only visible if any value differs from factory defaults
- Action: Resets all values to factory defaults after confirmation

## Editing Values

1. Select a parameter by pressing the rotary encoder button.
2. The display will invert colors to indicate edit mode.
3. Rotate the encoder to adjust the value.
4. Press the rotary encoder button again to confirm and exit edit mode.

## Saving Changes

- Changes are applied immediately when exiting edit mode for each parameter.
- To persist changes across power cycles, use the "Save EEPROM" option.
- If you exit the Settings menu without saving, changes will be lost on next power cycle.

## Factory Reset Procedure

1. Navigate to the "Factory Reset" option in the Settings menu.
2. Press the rotary encoder button to select.
3. Confirm the action when prompted.
4. All settings will be reset to their default values and saved to EEPROM.

## Tips for Optimal Configuration

- Adjust the Cook Time based on your specific cooking requirements.
- Set the Total Distance according to the physical constraints of your setup.
- Adjust Max Speed carefully, considering the mechanical limitations of your system.
- Always test the system after making significant changes to ensure proper operation.

Remember to save your settings to EEPROM after making changes you want to keep. This configuration system allows for flexible adjustment of the Marsh Mellow 2.0's behavior to suit various operational needs.
