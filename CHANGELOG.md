# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0-141] - 2024-08-23

This release introduces WS2812B LED strip functionality to the project, enhancing visual feedback and user experience. The LED strip is initialized to display a solid orange color at full brightness when the system starts up. This addition provides a clear visual indicator that the system is powered on and ready for operation.

### Added
- Added function to initialize and turn on WS2812B LED strip
- Added LED strip initialization to setup function
- Added constant definitions for WS2812B LED strip (NUM_LEDS, LED_TYPE, COLOR_ORDER)
- Added FastLED library integration for controlling the LED strip

### Changed
- Updated CONVENTIONS.md with guidance for working with WS2812B LED strip
- Modified setup() function to initialize the LED strip

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- No changes

### Security
- No changes

## [0.3.0-139] - 2024-08-23

This update includes a fix for an issue in the installation files.

### Added
- No changes

### Changed
- No changes

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- Resolved an issue in the installation files (Install_Firmware.cmd, Install_Firmware.ps1, and Install_Firmware.sh)

### Security
- No changes

## [0.3.0-134] - 2024-08-23

This update includes improvements in the installation process and documentation.

### Added
- Added instructions for debug firmware in INSTALL.md

### Changed
- Updated INSTALL.md with detailed instructions for installing Python and esptool on both Windows and Linux
- Changed firmware distribution to use zip files (firmware.zip and firmware-debug.zip) instead of individual files
- Updated installation scripts (Install_Firmware.cmd, Install_Firmware.ps1, and Install_Firmware.sh) to handle automatic port scanning

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- No changes

### Security
- No changes

## [0.3.0-120] - 2024-08-23

This release introduces debug firmware and updates the release naming convention.

### Added
- Debug firmware is now included in releases as a separate zip file.

### Changed
- Release naming convention updated to use semantic versioning (e.g., v1.2.3).

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- No changes

### Security
- No changes

## [0.3.0-115] - 2024-08-23

This release includes improvements to the ButtonHandler class, enhancing its functionality and debugging capabilities.

### Added
- New method `isPressedForMs()` in ButtonHandler class to check how long a button has been pressed.

### Changed
- Updated ButtonHandler class to track press start time.
- Improved debugging output in ButtonHandler class.

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- No changes

### Security
- No changes

## [0.3.0-96] - 2024-08-23

This release introduces an alternative firmware flashing method using esptool.py, providing users with a more flexible and robust way to update their Skumfidus devices. The INSTALL.md file has been significantly updated with detailed instructions for flashing firmware on various operating systems, including Windows (using both cmd and PowerShell) and Unix-based systems. This update aims to improve the user experience and make the firmware update process more accessible to a wider range of users. Additionally, extra files have been copied to the assets in the release for improved accessibility.

### Added
- Firmware flashing method using esptool.py in INSTALL.md
- Detailed instructions for flashing firmware on Windows (cmd and PowerShell) and Unix systems
- Added variables for COM_PORT and BAUD_RATE in flashing instructions
- Extra files copied to the assets in the release

### Changed
- Updated INSTALL.md with more comprehensive firmware flashing instructions

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- Wrong homefolder location in build file

### Security
- No changes

## [0.3.0-87] - 2024-08-23

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

## [0.3.0-85] - 2024-08-22

This release includes improvements to EEPROM usage and settings management.

### Added
- No changes

### Changed
- Improved EEPROM usage in Settings.cpp
- Enhanced variable size and offset calculations for better maintainability

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- Ensured EEPROM variable sizes fit allocated addresses

### Security
- No changes

## [0.3.0-12] - 2024-08-22

Updated the release template text, it was not that good..

### Added
- No changes

### Changed
- No changes
  
### Deprecated
- No changes
  
### Removed
- No changes
  
### Fixed
- Updated so the version is in the filename
- Updated the body in release so its prettier

### Security
- No changes

## [0.3.0-6] - 2024-08-22

Updated the build process and simplified it

### Added
- No changes

### Changed
- No changes

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- Fixed issue with version on GitVersion

### Security
- No changes

## [0.2.0] - 2024-08-22

Test release.. 

### Added
- No changes
  
### Changed
- Updated GitHub Actions to use the newest current versions
- Switched to using GitVersion for automatic version calculation based on commit history
- Added validation of the ChangeLog file to the GitHub Actions build process

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- Fixed a path in the semver action

### Security
- No changes

## [Initial] - 2024-08-22

### Added
- Initial release of Marsh Mellow 2.0 project
- ESP32-based control system for stepper motor and relay
- 16x2 I2C LCD screen for real-time information display
- Rotary encoder for user input in settings menu
- FreeRTOS implementation for multitasking
- EEPROM storage for persistent settings
- Configurable parameters: Cook Time, Total Distance, Max Speed
- State machine architecture with states: STARTUP, HOMING, IDLE, RUNNING, RETURNING_TO_START, ERROR, SETTINGS_MENU
- Precise stepper motor control with TMC2209 driver
- Periodic relay activation
- Comprehensive documentation including System Overview, Hardware Setup, Software Architecture, User Interface, Configuration Guide, and Troubleshooting
- Automated build process using GitHub Actions
- Automatic version calculation based on commit history

### Changed
- N/A (Initial release)

### Deprecated
- N/A (Initial release)

### Removed
- N/A (Initial release)

### Fixed
- N/A (Initial release)

### Security
- N/A (Initial release)
