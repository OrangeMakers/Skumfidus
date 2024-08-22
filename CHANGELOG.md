# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

### Changed
- Updated GitHub Actions to use the newest current versions

### Deprecated

### Removed

### Fixed

### Security

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
