# Pin Connections for ESP32, Stepper Motor, and LED

This document outlines the pin connections between the ESP32, the stepper motor driver, and the built-in LED.

## Stepper Motor Driver Connections

| ESP32 Pin | Board Label | Stepper Driver Pin |
|-----------|-------------|-------------------|
| PIN 13    | D13         | STEP              |
| PIN 12    | D12         | DIR               |
| 3.3V      | 3V3         | VCC (if the driver accepts 3.3V logic, otherwise use 5V) |
| GND       | GND         | GND               |

## Built-in LED Connection

| ESP32 Pin | Board Label | Connection |
|-----------|-------------|------------|
| PIN 2     | D2          | Built-in LED |

## LCD Display Connections

| ESP32 Pin | Board Label | LCD Pin |
|-----------|-------------|---------|
| PIN 21    | SDA         | SDA     |
| PIN 22    | SCL         | SCL     |
| 5V        | 5V          | VCC     |
| GND       | GND         | GND     |

## Relay Connection

| ESP32 Pin | Board Label | Connection |
|-----------|-------------|------------|
| PIN 14    | D14         | Relay Control |

## Notes

1. The stepper motor used is a 42hs3417 model.
2. Make sure to connect the stepper motor to the driver according to the driver's specifications.
3. The stepper driver is set to 8 microstepping mode. Ensure that MS1, MS2, and MS3 are set correctly on your stepper driver to achieve 8 microstepping.
4. Ensure that the stepper driver is powered appropriately. The power for the motor should come from a separate power supply, not from the ESP32.
5. Always double-check the voltage requirements of your specific stepper driver before making connections.
6. The built-in LED on most ESP32 development boards is connected to GPIO 2 and is active-low (it turns on when the pin is set to LOW).
7. The LCD display is connected via I2C. Make sure to connect it to the correct I2C pins (SDA and SCL) on the ESP32.
8. The LCD display used is a 16x2 character LCD with I2C interface. The I2C address is set to 0x27 in the code. If your display uses a different address, you may need to adjust this in the `src/main.cpp` file.
9. The relay is controlled by GPIO 14. Make sure to connect the relay's control pin to this GPIO.
10. Ensure that the relay is powered appropriately. If it requires more current than the ESP32 can provide, use a separate power supply and appropriate transistor or optocoupler for isolation.

## Code Configuration

In the `src/main.cpp` file, the following pins are defined:

```cpp
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2
#define RELAY_PIN 14
```

These definitions correspond to the physical connections described in the tables above.

The stepper motor (42hs3417) is configured to run at 2 revolutions per second with 8 microstepping:

```cpp
const int STEPS_PER_REV = 1600; // 200 * 8 (for 8 microstepping)
stepper.setMaxSpeed(3200);  // 2 revolutions per second
```

The motor is controlled using the AccelStepper library, which handles the precise timing for stepping.

The relay is controlled by a separate FreeRTOS task, which turns it on for 10 seconds and off for 10 seconds in a continuous cycle.
