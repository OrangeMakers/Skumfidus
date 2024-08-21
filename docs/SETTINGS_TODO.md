# Settings Implementation Todo List

## Implementation Guidelines
- Follow the guidance from SETTINGS.md for implementation details
- Ensure all dependencies and requirements for the feature are included
- Update header files and other necessary files for full functionality
- Maintain consistency with existing code structure and naming conventions
- Implement all menu items as described in SETTINGS.md
- Follow the interaction flow outlined in SETTINGS.md

## 1. Create Settings Class
- [x] Create `Settings.h` and `Settings.cpp` files
- [x] Define Settings class with necessary member variables and methods
- [x] Implement constructor and destructor

## 2. Implement Menu Structure
- [x] Define enum for menu items
- [x] Create array or vector to store menu items
- [x] Implement method to display current menu item

## 3. Encoder Integration
- [x] Add ESP32Encoder as a member variable in Settings class
- [x] Initialize encoder in constructor
- [x] Implement method to read encoder value and calculate delta

## 4. Navigation Logic
- [x] Implement method to handle menu navigation based on encoder input
- [x] Add logic to select menu item when rotary switch is pressed

## 5. Edit Mode
- [x] Implement enter/exit edit mode functionality
- [x] Add logic to adjust values based on encoder input in edit mode
- [x] Implement method to invert display colors for edit mode

## 6. Menu Items Implementation
- [x] Implement Cook Time adjustment
- [x] Implement Total Distance adjustment
- [x] Implement Max Speed adjustment
- [x] Add Load EEPROM functionality
- [x] Add Save EEPROM functionality
- [x] Implement Exit functionality
- [x] Add Factory Reset option

## 7. State Tracking
- [x] Add boolean flags for each editable value to track changes
- [x] Implement logic to show/hide Load/Save EEPROM and Factory Reset options

## 8. Recalculation
- [ ] Implement method to recalculate TOTAL_STEPS after changing Total Distance

## 9. Display Handling
- [x] Modify OMDisplay class to add enable/disable update method
- [x] Implement direct LCD control in Settings class

## 10. Integration with Main Program
- [x] Add Settings instance in main.cpp
- [x] Implement enterSettingsMenu() and exitSettingsMenu() functions
- [x] Add SETTINGS_MENU state to main loop
- [x] Implement logic to enter Settings menu from IDLE state

## 11. EEPROM Integration
- [x] Implement methods to load values from EEPROM
- [x] Implement methods to save values to EEPROM

## 12. Testing and Debugging
- [ ] Test each menu item functionality
- [ ] Verify navigation and edit mode behavior
- [ ] Test EEPROM load/save operations
- [ ] Debug any issues found during testing

## 13. Documentation
- [ ] Add comments to new code
- [ ] Update README or other documentation to reflect new Settings functionality

## 14. Code Review and Optimization
- [ ] Review entire implementation for consistency and best practices
- [ ] Optimize code where possible
- [ ] Ensure proper error handling and edge case management
