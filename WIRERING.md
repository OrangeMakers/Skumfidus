# Wiring Diagram for ESP32, TMC2209 Stepper Driver, Stepper Motor, LCD, Relay, and Start Button

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
| D15 (PIN 15) <----- Start Button (PULL UP, see separate component below)
| D16 (PIN 16) <----- Homing Switch (PULL UP)
| D17 (PIN 17) <---> Rotary Encoder CLK (PULL UP)
| D18 (PIN 18) <---> Rotary Encoder DT (PULL UP)
| D19 (PIN 19) <---> Rotary Encoder SW (PULL UP)
| 5V           -----> Common 5V (for LCD, TMC2209, Rotary Encoder, and other components)
| GND          -----> Common GND
|                     |
+---------------------+

Simple Diagram:

ESP32          TMC2209         Steppermotor     Rotary Encoder
+--------+     +--------+     +-------------+   +-------------+
|    D13 |---->| STEP   |     |             |   |             |
|    D12 |---->| DIR    |     |             |   |             |
|     5V |---->| VCC_IO |     |             |   |             |
|     5V |---->| VM     |     |             |   |             |
|    GND |---->| GND    |     |             |   |             |
|        |     | A1     |---->| Coil A (Red)|   |             |
|        |     | A2     |---->| Coil A (Blue)|  |             |
|        |     | B1     |---->| Coil B (Black)| |             |
|        |     | B2     |---->| Coil B (Green)| |             |
|        |     +--------+     +-------------+   |             |
|        |                                      |             |
|    SDA |---->| LCD SDA |                      |             |
|    SCL |---->| LCD SCL |                      |             |
|     5V |---->| LCD VCC |                      |             |
|    GND |---->| LCD GND |                      |             |
|        |                                      |             |
|    D14 |---->| Relay Control |                |             |
|        |                                      |             |
|    D15 |<----| Start Button |                 |             |
|    D16 |<----| Homing Switch |                |             |
|    D17 |<----------------------------------->| CLK         |
|    D18 |<----------------------------------->| DT          |
|    D19 |<----------------------------------->| SW          |
|     5V |-------------------------------------->| VCC         |
|    GND |<------------------------------------| GND         |
+--------+                                      +-------------+

+---------------------+
| Rotary Encoder      |
+---------------------+
| CLK <-------------> ESP32 D17 (PIN 17)
| DT  <-------------> ESP32 D18 (PIN 18)
| SW  <-------------> ESP32 D19 (PIN 19)
| + (VCC) <---------> Common 5V
| GND <-------------> Common GND
|                     |
+---------------------+

+---------------------+
| Homing Switch       |
+---------------------+
| Pin 1 <-----------> ESP32 D16 (PIN 16)
| Pin 2 <-----------> Common GND
|                     |
+---------------------+

+---------------------+
|      TMC2209        |
+---------------------+
| STEP <------------- ESP32 D13
| DIR  <------------- ESP32 D12
| EN   <------------- GND
| MS1  <------------- GND (for full-step mode)
| MS2  <------------- GND (for full-step mode)
| VCC_IO <----------- Common 5V (same as other components)
| VM   <------------- Common 5V (same as other components)
| GND  <------------- Common GND
| A1   <------------- Stepper Motor Coil A (Red)
| A2   <------------- Stepper Motor Coil A (Blue)
| B1   <------------- Stepper Motor Coil B (Black)
| B2   <------------- Stepper Motor Coil B (Green)
|                     |
+---------------------+

+---------------------+
|    Start Button     |
+---------------------+
| Pin 1 <-----------> ESP32 D15 (PIN 15)
| Pin 2 <-----------> Common GND
|                     |
+---------------------+

Note: The start button is a normally open (NO) momentary push button. When pressed, it connects D15 to GND, triggering the start/stop function in the software.

Notes:
1. Ensure all GND connections are properly connected to a common ground, including the relay module's GND.
2. The Start Button should be connected between PIN 15 and GND, with the internal pull-up resistor enabled in software (INPUT_PULLUP).
3. The Relay Control (PIN 14) should be connected to the control pin of your relay module.
4. The LCD and other components are powered from the common 5V supply.
5. The Stepper Motor connections (A1, A2, B1, B2) go to the TMC2209 driver, not directly to the ESP32.
6. Set the TMC2209 current using the onboard potentiometer. Current (RMS) = Vref * 1.77
7. For full-step mode (lowest resolution), connect both MS1 and MS2 to GND.
8. The motor power supply (VM) for the TMC2209 uses the common 5V supply, which should be sufficient for this stepper motor.
9. Double-check all connections before powering on the system.
10. Refer to the TMC2209 datasheet for additional features and configuration options.
11. Add a capacitor (typically 100μF to 470μF) between the power supply's GND and + terminals for noise reduction and power supply stabilization.

This wiring diagram provides a text-based representation of how to connect the various components to the ESP32 and the TMC2209 stepper driver. Always refer to the specific datasheets of your components for any additional connections or requirements not shown here. The added capacitor helps to reduce noise and stabilize the power supply, which can improve overall system performance and reliability.
