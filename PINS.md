# Pin Connections for ESP32, Stepper Motor, and LED

This document outlines the pin connections between the ESP32, the stepper motor driver, and the built-in LED.

## Stepper Motor Driver Connections

| ESP32 Pin | Board Label | Stepper Driver Pin |
|-----------|-------------|-------------------|
| GPIO 13   | D13         | STEP              |
| GPIO 12   | D12         | DIR               |
| 3.3V      | 3V3         | VCC (if the driver accepts 3.3V logic, otherwise use 5V) |
| GND       | GND         | GND               |

## Built-in LED Connection

| ESP32 Pin | Board Label | Connection |
|-----------|-------------|------------|
| GPIO 2    | D2          | Built-in LED |

## Notes

1. Make sure to connect the stepper motor to the driver according to the driver's specifications.
2. The stepper driver may require additional connections (like MS1, MS2, MS3 for microstepping), which are not shown here as they are not directly connected to the ESP32.
3. Ensure that the stepper driver is powered appropriately. The power for the motor should come from a separate power supply, not from the ESP32.
4. Always double-check the voltage requirements of your specific stepper driver before making connections.
5. The built-in LED on most ESP32 development boards is connected to GPIO 2 and is active-low (it turns on when the pin is set to LOW).

## Code Configuration

In the `src/main.cpp` file, the following pins are defined:

```cpp
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2
```

These definitions correspond to the physical connections described in the tables above.
