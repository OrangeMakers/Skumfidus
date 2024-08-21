# Troubleshooting Guide

This guide provides solutions for common issues that may arise while operating the Marsh Mellow 2.0 system. If you encounter a problem not listed here, please contact technical support.

## Common Issues and Solutions

### 1. System Doesn't Start

**Symptoms:**
- LCD doesn't light up
- No response from buttons or encoder

**Possible Causes and Solutions:**
1. **Power Issue**
   - Check if the power supply is connected and functioning.
   - Verify the voltage output of the power supply.

2. **ESP32 Not Booting**
   - Check if the ESP32 is properly seated in its socket.
   - Try reflashing the firmware.

3. **Wiring Problem**
   - Inspect all connections for loose wires or short circuits.
   - Verify that all components are connected to the correct pins.

### 2. Stepper Motor Not Moving

**Symptoms:**
- Motor doesn't move during homing or normal operation
- System enters ERROR state when trying to move

**Possible Causes and Solutions:**
1. **Driver Configuration**
   - Verify that the TMC2209 driver is properly configured.
   - Check if the driver is receiving the correct signals from the ESP32.

2. **Motor Wiring**
   - Ensure that the motor is correctly wired to the driver.
   - Check for any loose connections.

3. **Current Setting**
   - Verify that the motor current is set correctly on the TMC2209 driver.

4. **Software Issue**
   - Check if the STEPS_PER_REV and other motor-related constants are set correctly in the code.

### 3. Homing Fails

**Symptoms:**
- System doesn't complete the homing sequence
- Enters ERROR state during homing

**Possible Causes and Solutions:**
1. **Homing Switch Problem**
   - Check if the homing switch is properly connected and functioning.
   - Verify the switch's wiring and connection to the ESP32.

2. **Incorrect Homing Direction**
   - Ensure that the HOMING_DIRECTION constant in the code is set correctly.

3. **Mechanical Obstruction**
   - Check for any physical obstructions preventing the motor from reaching the home position.

### 4. LCD Display Issues

**Symptoms:**
- LCD doesn't show any text
- Display shows garbled characters

**Possible Causes and Solutions:**
1. **I2C Connection**
   - Verify that the SDA and SCL pins are correctly connected.
   - Check if the I2C address in the code matches the LCD's address.

2. **Power Issue**
   - Ensure that the LCD is receiving the correct voltage.

3. **Software Problem**
   - Check if the LCD library is properly initialized in the code.

### 5. Settings Not Saving

**Symptoms:**
- Changed settings revert after power cycle

**Possible Causes and Solutions:**
1. **EEPROM Writing Issue**
   - Verify that the EEPROM.commit() function is called after writing settings.
   - Check if the EEPROM is properly initialized in the setup() function.

2. **Incorrect Loading**
   - Ensure that the loadSettingsFromEEPROM() function is called during startup.

### 6. Rotary Encoder Not Responding

**Symptoms:**
- Unable to navigate settings menu
- Encoder rotation not detected

**Possible Causes and Solutions:**
1. **Wiring Issue**
   - Check if the encoder is properly connected to the correct pins.
   - Verify that the encoder's common pin is connected to ground.

2. **Software Configuration**
   - Ensure that the encoder pins are correctly defined in the code.
   - Verify that the encoder library is properly initialized.

### 7. Relay Not Switching

**Symptoms:**
- Relay doesn't turn on/off as expected

**Possible Causes and Solutions:**
1. **Wiring Problem**
   - Check if the relay module is properly connected to the ESP32.
   - Verify that the relay is receiving the correct control signal.

2. **Software Issue**
   - Ensure that the relay control pin is correctly defined and initialized in the code.
   - Check if the relay control logic in the main loop is functioning as expected.

## General Troubleshooting Steps

1. **Check Connections:** Always start by verifying all electrical connections.
2. **Restart the System:** Sometimes, a simple power cycle can resolve issues.
3. **Check Serial Output:** If available, monitor the serial output for error messages or unexpected behavior.
4. **Verify Settings:** Ensure all settings are within their expected ranges.
5. **Inspect Mechanical Components:** Check for any physical obstructions or wear in moving parts.

If problems persist after trying these solutions, please consult the technical documentation or contact support for further assistance.
