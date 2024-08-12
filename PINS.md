# Pin Connections for ESP32 and Stepper Motor

This document outlines the pin connections between the ESP32 and the stepper motor driver.

## Stepper Motor Driver Connections

| ESP32 Pin | Stepper Driver Pin |
|-----------|-------------------|
| GPIO 13   | STEP              |
| GPIO 12   | DIR               |
| 3.3V      | VCC (if the driver accepts 3.3V logic, otherwise use 5V) |
| GND       | GND               |

## Notes

1. Make sure to connect the stepper motor to the driver according to the driver's specifications.
2. The stepper driver may require additional connections (like MS1, MS2, MS3 for microstepping), which are not shown here as they are not directly connected to the ESP32.
3. Ensure that the stepper driver is powered appropriately. The power for the motor should come from a separate power supply, not from the ESP32.
4. Always double-check the voltage requirements of your specific stepper driver before making connections.

## Code Configuration

In the `src/main.cpp` file, the following pins are defined for the stepper motor:

```cpp
#define STEP_PIN 13
#define DIR_PIN 12
```

These definitions correspond to the physical connections described in the table above.
