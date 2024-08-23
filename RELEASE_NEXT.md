## Description of next release

[Insert release description here]

### Added
- Added `startHeater` function to control the relay and built-in LED for heating
- Added `stopHeater` function to turn off the relay and built-in LED
- Added debug messages for heater status when DEBUG is defined
- Implemented automatic heater start when entering the running state
- Implemented a "park at back" feature, activated by holding the start button for 5 seconds in IDLE state
- New PARKING state to handle the parking process
- Stepper motor moves to 120mm position when parking
- System displays "Please turn off The power" message after parking and stops all processing

### Changed
- Implemented generic direction constants `DIRECTION_HOME` and `DIRECTION_RUN` for improved code clarity and maintainability
- Enhanced safety by ensuring the heater is stopped when cooking process is interrupted, completed, or an error occurs
- Modified IDLE state handling to accommodate both normal start and long-press for parking

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- No changes

### Security
- No changes
