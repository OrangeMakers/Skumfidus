#include <Arduino.h>
#include <AccelStepper.h>

// Define stepper motor connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32

// Define steps per revolution for 1/2 microstepping
const int STEPS_PER_REV = 400; // 200 * 2 (for 1/2 microstepping)

// Create a new instance of the AccelStepper class
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(1000); // Increased to allow for higher speeds
  stepper.setAcceleration(500); // Increased for faster acceleration
  
  // Set the speed (steps per second)
  // 400 steps per revolution (with 1/2 microstepping), 1 revolution per second
  stepper.setSpeed(STEPS_PER_REV);

  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Move the motor one step
  stepper.runSpeed();

  // Toggle the LED state
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
