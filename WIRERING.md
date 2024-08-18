# Wiring Diagram for ESP32, Stepper Motor, LCD, Relay, and Start Button

```
+---------------------+
|       ESP32         |
+---------------------+
|                     |
| D13 (PIN 13) -----> Stepper Motor STEP
| D12 (PIN 12) -----> Stepper Motor DIR
| D2  (PIN 2)  -----> Built-in LED
| SDA (PIN 21) -----> LCD SDA
| SCL (PIN 22) -----> LCD SCL
| D14 (PIN 14) -----> Relay Control
| D15 (PIN 15) <----- Start Button
| 3V3          -----> Stepper Driver VCC
| 5V           -----> Common 5V (for LCD and other components)
| GND          -----> Common GND
|                     |
+---------------------+

Notes:
1. Ensure all GND connections are properly connected to a common ground.
2. The Start Button should be connected between PIN 15 and GND, with the internal pull-up resistor enabled in software.
3. The Relay Control (PIN 14) should be connected to the control pin of your relay module.
4. The LCD and other components are powered from the common 5V supply.
5. The Stepper Motor connections (STEP and DIR) go to your stepper driver, not directly to the motor.
6. Double-check all connections before powering on the system.
```

This wiring diagram provides a text-based representation of how to connect the various components to the ESP32. Always refer to the specific datasheets of your components for any additional connections or requirements not shown here.
