# Software Architecture

The Marsh Mellow 2.0 project uses a state machine architecture implemented in C++ using the Arduino framework and FreeRTOS for task management. This document outlines the key components and their interactions within the software system.

## Main Components

1. **State Machine**: Manages the overall system state and transitions.
2. **Stepper Motor Control**: Handles precise movement of the stepper motor.
3. **Display Management**: Controls the LCD display updates.
4. **Settings System**: Manages user-configurable settings.
5. **Input Handling**: Processes user inputs from buttons and rotary encoder.
6. **Timer Management**: Handles timing-related functions.

## Key Classes and Their Responsibilities

### 1. Main Application (main.cpp)
- Initializes hardware components
- Implements the main state machine
- Coordinates interactions between different components

### 2. MatrixDisplay
- Manages LCD display updates
- Provides thread-safe display update mechanism

### 3. Settings
- Handles user-configurable settings
- Manages settings menu navigation and editing
- Interfaces with EEPROM for persistent storage

### 4. ButtonHandler
- Manages button inputs with debounce logic
- Provides methods to check button states

### 5. Timer
- Implements a simple timer functionality
- Used for timing various operations in the system

### 6. AccelStepper (External Library)
- Controls the stepper motor
- Manages acceleration and deceleration profiles

## State Machine

The system operates in the following states:

1. STARTUP
2. HOMING
3. IDLE
4. RUNNING
5. RETURNING_TO_START
6. ERROR
7. SETTINGS_MENU

Each state has its own handler function in the main loop, responsible for state-specific behaviors and transitions.

## Multitasking with FreeRTOS

The project utilizes FreeRTOS for task management:

1. **Main Loop Task**: Handles the state machine and overall system control.
2. **Display Update Task**: Manages LCD updates in a separate thread.
3. **Settings Update Task**: Handles settings menu updates when active.

## Key Algorithms

1. **Stepper Motor Control**: Uses AccelStepper library for smooth acceleration and deceleration.
2. **Display Update**: Implements a thread-safe buffer system for efficient LCD updates.
3. **Settings Management**: Uses a menu-based system with rotary encoder input for navigation and editing.
4. **Debounce Logic**: Implemented in ButtonHandler for reliable button input processing.

## Data Flow

1. User input (buttons, rotary encoder) → Input Handling → State Machine
2. State Machine → Stepper Motor Control, Display Management, Settings System
3. Settings System ↔ EEPROM (for persistent storage)
4. Stepper Motor Control → Physical stepper motor movement
5. Display Management → LCD screen updates

## Error Handling

- The ERROR state in the state machine handles various error conditions.
- Each component implements error checking and reporting mechanisms.

## Future Improvements

- Implement more robust error handling and recovery mechanisms.
- Enhance the settings system with more configuration options.
- Optimize memory usage and task scheduling for better performance.

For detailed implementation of each component, refer to the respective source files in the project repository.
