# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0-12] - 2024-08-22

Updated the release template text, it was not that good..

### Added

### Changed

### Deprecated

### Removed

### Fixed
- Updated so the version is in the filename
- Updated the body in release so its prettier

### Security

## [0.3.0-6] - 2024-08-22

Updated the build process and simplified it

### Added
- [Update Additions]

### Changed
- Changed to maintained github actions

### Deprecated
- [Update Deprecations]

### Removed
- [Update Removals]

### Fixed
- Fixed issue with version on GitVersion

### Security
- [Update Security Fixes]

## [0.2.0] - 2024-08-22

Test release.. 

### Added
- [Update Additions]
  
### Changed
- Updated GitHub Actions to use the newest current versions
- Switched to using GitVersion for automatic version calculation based on commit history
- Added validation of the ChangeLog file to the GitHub Actions build process

### Deprecated
- [Update Deprecations]

### Removed
- [Update Removals]

### Fixed
- Fixed a path in the semver action

### Security
- [Update Security Fixes]

## [] - 2024-08-22

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
