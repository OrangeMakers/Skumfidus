## Description of next release

This release introduces significant improvements to the Marshmallow 2.0 system, including enhanced user interface, safety features, and system stability.

### Added
- Implemented a Settings menu accessible by long-pressing the rotary switch in IDLE state
- Added `Settings` class to manage and persist user-configurable parameters
- Introduced a queueing system for display messages to improve UI responsiveness
- Added `MatrixDisplay` class for more efficient LCD updates
- Implemented "park at back" feature, activated by holding the start button for 5 seconds in IDLE state
- Added new PARKING state to handle the parking process
- Implemented automatic heater control (start/stop) based on system state
- Added debug messages for heater status and current settings when DEBUG is defined

### Changed
- Refactored main loop to use a state machine approach for improved code organization
- Enhanced IDLE state handling to accommodate normal start, long-press for parking, and entering settings menu
- Improved error handling and reporting across all states
- Modified stepper motor control to use direction constants (DIRECTION_HOME and DIRECTION_RUN) for better maintainability
- Updated LCD display to show real-time information during cooking process (time remaining and distance)
- Enhanced safety by ensuring the heater is stopped when cooking process is interrupted, completed, or an error occurs

### Deprecated
- No changes

### Removed
- Removed hardcoded values for cook time, total distance, and speed, now managed through Settings

### Fixed
- Improved reliability of homing process with better error handling and timeout
- Fixed potential issues with button debouncing and encoder reading

### Security
- No changes
