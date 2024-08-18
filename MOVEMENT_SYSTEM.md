# Movement System Specifications and Setup Guide

## 1. Stepper Motor Specifications (42HB34F08AB)

### General Information
- **Model**: 42HB34F08AB
- **Type**: Hybrid Stepper Motor
- **Manufacturer**: Changzhou Bo Hong Electric Appliance Co.,Ltd

### Electrical Specifications
- **Rated Voltage**: 4.96V
- **Rated Current**: 0.8A
- **Phase Resistance**: 6.2Ω ±10%
- **Phase Inductance**: 10mH ±20%

### Mechanical Specifications
- **Step Angle**: 1.8° ±5% (full step, no load)
- **Holding Torque**: 2.4 kg·cm
- **Number of Lead Wires**: 4
- **Rotor Inertia**: 34 g·cm²
- **Motor Weight**: 200g
- **Motor Length**: 34mm

### Performance Specifications
- **Temperature Rise**: 80°C Max. (rated current, 2 phase on)
- **Ambient Temperature Range**: -20°C ~ +50°C
- **Insulation Resistance**: 100MΩ Min., 500VDC
- **Dielectric Strength**: 500VAC for one minute

### Mechanical Tolerances
- **Shaft Radial Play**: 0.02mm Max. (450 g-load)
- **Shaft Axial Play**: 0.08mm Max. (450 g-load)
- **Max. Radial Force**: 28N (20mm from the flange)
- **Max. Axial Force**: 10N

## 2. TMC2209 Stepper Motor Driver Setup and Configuration

### Hardware Setup

#### Wiring
- **VM** (4.75V - 28V DC): Motor power supply
- **GND**: Ground
- **A1, A2**: Motor coil A
- **B1, B2**: Motor coil B
- **VCC_IO** (3.3V or 5V): Logic voltage
- **EN**: Enable pin (active low)
- **STEP**: Step input
- **DIR**: Direction input
- **MS1, MS2**: Microstep configuration

#### Current Setting
- Use the onboard potentiometer to adjust Vref
- Current (RMS) = Vref * 1.77
- Start with a low current and increase as needed

### Microstepping Configuration

For the lowest resolution (as requested), use full-step mode:

| MS1 | MS2 | Microstep Resolution |
|-----|-----|----------------------|
| GND | GND | Full step (lowest resolution) |

Connect both MS1 and MS2 to GND for full-step mode.

### Pin Configuration

- Configure ENABLE, STEP, and DIR pins as outputs on your microcontroller
- Connect these pins to the corresponding inputs on the TMC2209

### Motor Control Concepts

#### Basic Operations
1. **Enable/Disable**: Control the EN pin to enable or disable the driver
2. **Direction**: Set the DIR pin to control motor direction
3. **Stepping**: Toggle the STEP pin to make the motor move

#### Movement Control
- Implement functions to control step count and timing
- Use delays or timers to control motor speed

### Tips for Implementation
- Start with slower speeds and increase gradually
- Monitor motor temperature during operation
- Implement a cool-down period for continuous operation
- Consider using interrupts or timers for more precise control

### Troubleshooting
- If the motor doesn't move, check wiring and ensure the driver is enabled
- If the motor is noisy, try adjusting the current
- For missed steps, increase the current or reduce speed

### Advanced Features (Optional)
- UART mode for advanced configuration
- StealthChop2™ for ultra-quiet operation
- SpreadCycle™ for highly dynamic motor control
- StallGuard4™ for stall detection
- CoolStep™ for dynamic current control

## 3. Setup and Calibration Notes

1. **Wiring**: 
   - The motor has 4 lead wires. Refer to the manufacturer's wiring diagram for proper connection.
   - Ensure correct phase sequence to achieve proper rotation direction.

2. **Current Setting**:
   - Set the driver current to 0.8A (rated current) for optimal performance.
   - Do not exceed the rated current to prevent overheating.

3. **Microstepping**:
   - The basic step angle is 1.8°, which equals 200 steps/revolution.
   - Use microstepping for higher resolution and smoother motion.

4. **Resonance**:
   - Be aware of resonance points, which may occur at certain speeds.
   - Use microstepping or damping techniques to minimize resonance effects.

5. **Temperature Monitoring**:
   - Monitor motor temperature during operation to ensure it stays within the specified range.
   - Implement cooling if necessary for continuous operation.

6. **Load Considerations**:
   - Ensure the applied load does not exceed the motor's max radial and axial force ratings.
   - The holding torque is 2.4 kg·cm; factor in safety margins when designing your application.

7. **Driver Compatibility**:
   - Use a driver compatible with the motor's electrical specifications (voltage and current ratings).

8. **Acceleration Profiles**:
   - Implement smooth acceleration and deceleration profiles to optimize performance and minimize wear.

Remember to refer to the complete datasheet and consult with the manufacturer for any specific applications or concerns not covered in this summary.

## 3. 8mm ACME Threaded Rod Specifications

### Basic Specifications
| Parameter | Value |
|-----------|-------|
| Nominal Diameter | 8mm |
| Thread Type | ACME (trapezoidal) |
| Lead | 8mm |
| Pitch | 2mm |
| Start | 4 (Multi-start thread) |

### Thread Geometry
| Parameter | Value |
|-----------|-------|
| Major Diameter | 8.00mm |
| Minor Diameter | 6.50mm |
| Pitch Diameter | 7.25mm |
| Thread Angle | 29° |
| Thread Depth | 0.75mm |

### Material Properties
| Parameter | Value |
|-----------|-------|
| Material | Stainless Steel (typical) |
| Surface Finish | Ground and Polished |

### Performance Characteristics
| Parameter | Value |
|-----------|-------|
| Typical Backlash | 0.05mm - 0.1mm (depends on nut design) |
| Efficiency | ~50% (typical for ACME threads) |

### Notes for Developers:
- Lead: The linear distance traveled for one complete revolution of the rod.
- Pitch: The distance between adjacent thread peaks.
- Start: The number of independent threads. In this case, 4 starts means there are four threads running in parallel.
- The lead is equal to the pitch multiplied by the number of starts (2mm * 4 = 8mm).
- For precise movement calculations, consider factors such as:
  1. Stepper motor step angle and microstepping settings
  2. Any gearing between the motor and the threaded rod
  3. Backlash in the system
  4. Potential thermal expansion of components
- The efficiency affects the torque required to lift a given load and the back-drivability of the system.
