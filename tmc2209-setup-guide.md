# TMC2209 Stepper Motor Driver Setup Guide

## 1. Hardware Setup

### Wiring
- **VM** (4.75V - 28V DC): Motor power supply
- **GND**: Ground
- **A1, A2**: Motor coil A
- **B1, B2**: Motor coil B
- **VCC_IO** (3.3V or 5V): Logic voltage
- **EN**: Enable pin (active low)
- **STEP**: Step input
- **DIR**: Direction input
- **MS1, MS2**: Microstep configuration

### Current Setting
- Use the onboard potentiometer to adjust Vref
- Current (RMS) = Vref * 1.77
- Start with a low current and increase as needed

## 2. Microstepping Configuration

For the lowest resolution (as requested), use full-step mode:

| MS1 | MS2 | Microstep Resolution |
|-----|-----|----------------------|
| GND | GND | Full step (lowest resolution) |

Connect both MS1 and MS2 to GND for full-step mode.

## 3. Pin Configuration

- Configure ENABLE, STEP, and DIR pins as outputs on your microcontroller
- Connect these pins to the corresponding inputs on the TMC2209

## 4. Motor Control Concepts

### Basic Operations
1. **Enable/Disable**: Control the EN pin to enable or disable the driver
2. **Direction**: Set the DIR pin to control motor direction
3. **Stepping**: Toggle the STEP pin to make the motor move

### Movement Control
- Implement functions to control step count and timing
- Use delays or timers to control motor speed

## 5. Tips for Implementation
- Start with slower speeds and increase gradually
- Monitor motor temperature during operation
- Implement a cool-down period for continuous operation
- Consider using interrupts or timers for more precise control

## 6. Troubleshooting
- If the motor doesn't move, check wiring and ensure the driver is enabled
- If the motor is noisy, try adjusting the current
- For missed steps, increase the current or reduce speed

## 7. Advanced Features (Optional)
- UART mode for advanced configuration
- StealthChop2™ for ultra-quiet operation
- SpreadCycle™ for highly dynamic motor control
- StallGuard4™ for stall detection
- CoolStep™ for dynamic current control

Remember to refer to the TMC2209 datasheet for detailed information on advanced features and configurations.
