# 42HB34F08AB-22 Stepper Motor Specifications

## General Information
- **Model**: 42HB34F08AB
- **Type**: Hybrid Stepper Motor
- **Manufacturer**: Changzhou Bo Hong Electric Appliance Co.,Ltd

## Electrical Specifications
- **Rated Voltage**: 4.96V
- **Rated Current**: 0.8A
- **Phase Resistance**: 6.2Ω ±10%
- **Phase Inductance**: 10mH ±20%

## Mechanical Specifications
- **Step Angle**: 1.8° ±5% (full step, no load)
- **Holding Torque**: 2.4 kg·cm
- **Number of Lead Wires**: 4
- **Rotor Inertia**: 34 g·cm²
- **Motor Weight**: 200g
- **Motor Length**: 34mm

## Performance Specifications
- **Temperature Rise**: 80°C Max. (rated current, 2 phase on)
- **Ambient Temperature Range**: -20°C ~ +50°C
- **Insulation Resistance**: 100MΩ Min., 500VDC
- **Dielectric Strength**: 500VAC for one minute

## Mechanical Tolerances
- **Shaft Radial Play**: 0.02mm Max. (450 g-load)
- **Shaft Axial Play**: 0.08mm Max. (450 g-load)
- **Max. Radial Force**: 28N (20mm from the flange)
- **Max. Axial Force**: 10N

## Setup and Calibration Notes

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
