# Marsh Mellow 2.0 System Flow

## System Boot Up

1. Initialize hardware
   - Set up pins
   - Initialize LCD
   - Configure stepper motor

2. Display welcome message
   - Reset display
   - Update display with text

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | OrangeMakers     |
| 2            | Marshmallow 2.0  |

   - Delay 5 seconds

3. Perform homing sequence
   - Move stepper motor towards home position
   - Monitor homing switch
   - When homing switch is triggered:
     - Stop motor
     - Set current position as zero (0.0 mm)
   - Display "Homing..." during the process
   - Display "Homed" for 2 seconds when complete

4. Initialize system state
   - Set system to IDLE state
   - Clear LCD and prepare for main display

5. Start background tasks
   - LCD update task
   - Relay control task

## Idle State

1. System waits for input
   - Display shows initial position (0.0 mm) and relay state (Off)

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Distance:        |
| 2            | 0.0 mm      Off  |

2. Stepper motor is stationary
3. Relay cycles on and off every 10 seconds (controlled by background task)

## Start Flow

1. User presses the start button
   - System detects button press (transition from HIGH to LOW)
   - Debounce for 50ms

2. System state changes from IDLE to RUNNING
   - Display temporary "System Started" message

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | System Started   |
| 2            |                  |

   - Display this message for 2 seconds

3. Begin main process
   - Stepper motor starts moving towards TOTAL_STEPS (30.0 mm)
   - LCD continuously updates to show current position and relay state

## Running State

1. Stepper Motor Operation
   - Motor moves towards target position (either 0 or TOTAL_STEPS)
   - When target reached, change direction and move to opposite end
   - Brief pause (500ms) when changing direction
   - Built-in LED toggles at each direction change

2. LCD Update
   - Continuously update distance and relay state
   - Update every 250ms (controlled by background task)

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | Distance:        |
| 2            | XX.X mm      On  |

   Note: XX.X represents the current distance (e.g., 15.2 mm)

3. Relay Operation
   - Continues to cycle on and off every 10 seconds (controlled by background task)

## Stop Flow

1. User presses the start button again
   - System detects button press (transition from HIGH to LOW)
   - Debounce for 50ms

2. System state changes from RUNNING to IDLE
   - Display temporary "System Idle" message

| Row / Column | 1234567890123456 |
| ------------ | ---------------- |
| 1            | System Idle      |
| 2            |                  |

   - Display this message for 2 seconds

3. Stop stepper motor
   - Reset stepper position to 0
   - Motor stops moving

4. Return to Idle State
   - System waits for next start command
   - LCD shows initial position (0.0 mm) and current relay state

Note: The relay control task continues to run in the background regardless of the system state (IDLE or RUNNING), ensuring that the relay cycles on and off every 10 seconds throughout the entire operation of the system.
