# Hardware Setup

This guide provides detailed information on the hardware components and their connections for the Marsh Mellow 2.0 project.

## Components List

1. ESP32 DevKit V1
2. 42HB34F08AB Stepper Motor
3. TMC2209 Stepper Motor Driver
4. 16x2 I2C LCD Screen
5. Relay Module
6. Rotary Encoder
7. Start Button
8. Homing Switch
9. 8mm ACME Threaded Rod

## Pin Connections

| ESP32 Pin | Board Label | Connection              |
|-----------|-------------|-------------------------|
| PIN 13    | D13         | Stepper Motor STEP      |
| PIN 12    | D12         | Stepper Motor DIR       |
| PIN 27    | D27         | Stepper Motor ENABLE    |
| PIN 2     | D2          | Built-in LED            |
| PIN 4     | D4          | Addressable LED         |
| PIN 21    | SDA         | LCD SDA                 |
| PIN 22    | SCL         | LCD SCL                 |
| PIN 14    | D14         | Relay Control           |
| PIN 15    | D15         | Start Button (PULL UP)  |
| PIN 16    | D16         | Homing Switch (PULL UP) |
| PIN 17    | D17         | Rotary Encoder CLK      |
| PIN 18    | D18         | Rotary Encoder DT       |
| PIN 19    | D19         | Rotary Encoder SW       |
| 5V        | 5V          | Power Supply for components |
| GND       | GND         | Common Ground           |

## Stepper Motor Specifications (42HB34F08AB)

- Type: Hybrid Stepper Motor
- Step Angle: 1.8° ±5% (full step, no load)
- Rated Voltage: 4.96V
- Rated Current: 0.8A
- Holding Torque: 2.4 kg·cm
- Number of Lead Wires: 4
- Wire Order: Red, Blue, Green, Black

## TMC2209 Stepper Driver Setup

- VM: 5V (Motor power supply)
- VCC_IO: 5V (Logic voltage)
- EN: Connected to ESP32 PIN 27
- STEP: Connected to ESP32 PIN 13
- DIR: Connected to ESP32 PIN 12
- MS1, MS2: Connected to GND for full-step mode

## 8mm ACME Threaded Rod Specifications

- Nominal Diameter: 8mm
- Thread Type: ACME (trapezoidal)
- Lead: 8mm
- Pitch: 2mm
- Start: 4 (Multi-start thread)

## Assembly Instructions

1. Mount the ESP32 DevKit V1 on your project board.
2. Connect the TMC2209 driver to the ESP32 and stepper motor according to the pin connections table.
3. Attach the LCD screen to the I2C pins (SDA and SCL) of the ESP32.
4. Connect the relay module to PIN 14 of the ESP32.
5. Install the rotary encoder and connect it to PINs 17, 18, and 19 of the ESP32.
6. Mount the start button and homing switch, connecting them to PINs 15 and 16 respectively.
7. Secure the stepper motor and attach it to the 8mm ACME threaded rod.
8. Ensure all components share a common ground connection.
9. Double-check all connections before powering on the system.

For detailed wiring diagrams and additional setup information, please refer to the `WIRERING.md` file in the project documentation.
