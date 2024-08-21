# System Overview

The Marsh Mellow 2.0 is an automated system designed to control a stepper motor and a relay using an ESP32 microcontroller. It features real-time information display on an LCD screen and uses FreeRTOS for multitasking.

## Key Components

1. **ESP32 DevKit V1**: The main microcontroller running the system.
2. **42HB34F08AB Stepper Motor**: Controlled for precise movements.
3. **16x2 I2C LCD Screen**: Displays real-time system information.
4. **Relay Module**: Periodically activated for additional control.
5. **TMC2209 Stepper Motor Driver**: Manages stepper motor control.
6. **Rotary Encoder**: Used for user input in the settings menu.

## Main Features

1. **Precise Stepper Motor Control**: Manages accurate movements with configurable distance and speed.
2. **Periodic Relay Activation**: Toggles a relay at set intervals.
3. **Real-time LCD Display**: Shows current distance, relay status, and system state.
4. **Settings Menu**: Allows user to configure system parameters via a rotary encoder.
5. **Multitasking**: Utilizes FreeRTOS for efficient task management.
6. **EEPROM Storage**: Saves and loads system settings for persistence across power cycles.

## System States

1. **STARTUP**: Initializes hardware and displays welcome message.
2. **HOMING**: Calibrates the stepper motor's position.
3. **IDLE**: Waits for user input to start operation.
4. **RUNNING**: Executes the main stepper motor movement and relay control.
5. **RETURNING_TO_START**: Moves the stepper back to the starting position.
6. **ERROR**: Handles and displays system errors.
7. **SETTINGS_MENU**: Allows user to modify system parameters.

This overview provides a high-level understanding of the Marsh Mellow 2.0 system. For detailed information on setup, configuration, and operation, please refer to the other documentation files.
