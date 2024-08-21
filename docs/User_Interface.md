# User Interface

The Marsh Mellow 2.0 project features a simple yet effective user interface consisting of an LCD display, a start button, and a rotary encoder. This document outlines the user interaction flow and display information for various system states.

## Input Devices

1. **Start Button**: Initiates and stops the main process.
2. **Rotary Encoder**: Used for navigating and adjusting settings.
   - Rotation: Scroll through menu items or adjust values.
   - Press: Select menu items or confirm changes.

## LCD Display

The 16x2 LCD screen provides real-time information about the system state, distance, and relay status.

### Display Format

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | State/Menu Item  |
| 2            | Value/Status     |

## System States and Corresponding Displays

### 1. Startup

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | OrangeMakers     |
| 2            | Marshmallow 2.0  |

- Displayed for 5 seconds during system initialization.

### 2. Homing

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Homing...        |
| 2            |                  |

- Displayed while the system is performing the homing sequence.

### 3. Idle

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Distance:        |
| 2            | 0.0 mm      Off  |

- Shows the current position and relay state.
- System is ready to start.

### 4. Running

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Distance:        |
| 2            | XX.X mm      On  |

- Displays real-time distance and relay state.
- XX.X represents the current distance (e.g., 15.2 mm).

### 5. Returning to Start

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Returning:       |
| 2            | XX.X mm          |

- Shows the distance while the system returns to the start position.

### 6. Error

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Error:           |
| 2            | [Error Message]  |

- Displays error messages when issues occur.

### 7. Settings Menu

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | [Setting Name]   |
| 2            | [Current Value]  |

- Shows the current setting being adjusted.

## Settings Menu Navigation

1. To enter the Settings menu:
   - From the Idle state, press and hold the rotary encoder button for 1 second.

2. Navigate through settings:
   - Rotate the encoder clockwise to move to the next item.
   - Rotate counterclockwise to move to the previous item.

3. Edit a setting:
   - Press the rotary encoder to enter edit mode.
   - Rotate to adjust the value.
   - Press again to confirm and exit edit mode.

4. Exit Settings menu:
   - Navigate to the "Exit" option and press the rotary encoder.

## User Interaction Flow

1. System starts up and performs homing.
2. In Idle state, press the Start button to begin the main process.
3. During Running state, press the Start button again to stop and return to Idle.
4. In Idle state, use the rotary encoder to enter and navigate the Settings menu.
5. Adjust settings as needed and exit the Settings menu.
6. Repeat the process as necessary.

This user interface design provides a straightforward way for users to control the system and adjust settings, with clear visual feedback through the LCD display.
