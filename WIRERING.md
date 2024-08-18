# Wiring Diagram for ESP32, TMC2209 Stepper Driver, Stepper Motor, LCD, Relay, and Start Button

```
+---------------------+
|       ESP32         |
+---------------------+
|                     |
| D13 (PIN 13) -----> TMC2209 STEP
| D12 (PIN 12) -----> TMC2209 DIR
| D2  (PIN 2)  -----> Built-in LED
| SDA (PIN 21) -----> LCD SDA
| SCL (PIN 22) -----> LCD SCL
| D14 (PIN 14) -----> Relay Control
| D15 (PIN 15) <----- Start Button
| 3V3          -----> TMC2209 VCC_IO
| 5V           -----> Common 5V (for LCD and other components)
| GND          -----> Common GND
|                     |
+---------------------+

+---------------------+
|      TMC2209        |
+---------------------+
| STEP <------------- ESP32 D13
| DIR  <------------- ESP32 D12
| EN   <------------- GND (or ESP32 pin for enable control)
| MS1  <------------- GND (for full-step mode)
| MS2  <------------- GND (for full-step mode)
| VCC_IO <----------- ESP32 3V3
| VM   <------------- Motor power supply (4.75V - 28V DC)
| GND  <------------- Common GND
| A1   <------------- Stepper Motor Coil A
| A2   <------------- Stepper Motor Coil A
| B1   <------------- Stepper Motor Coil B
| B2   <------------- Stepper Motor Coil B
|                     |
+---------------------+

Notes:
1. Ensure all GND connections are properly connected to a common ground, including the relay module's GND.
2. The Start Button should be connected between PIN 15 and GND, with the internal pull-up resistor enabled in software.
3. The Relay Control (PIN 14) should be connected to the control pin of your relay module.
4. The LCD and other components are powered from the common 5V supply.
5. The Stepper Motor connections (A1, A2, B1, B2) go to the TMC2209 driver, not directly to the ESP32.
6. Set the TMC2209 current using the onboard potentiometer. Current (RMS) = Vref * 1.77
7. For full-step mode (lowest resolution), connect both MS1 and MS2 to GND.
8. The motor power supply (VM) for the TMC2209 should be appropriate for your stepper motor (4.75V - 28V DC).
9. Double-check all connections before powering on the system.
10. Refer to the TMC2209 datasheet for additional features and configuration options.
```

This wiring diagram provides a text-based representation of how to connect the various components to the ESP32 and the TMC2209 stepper driver. Always refer to the specific datasheets of your components for any additional connections or requirements not shown here.
