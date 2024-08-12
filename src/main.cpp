#include <Arduino.h>
#include <AccelStepper.h>

// Define stepper motor connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32

// Create a new instance of the AccelStepper class
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(200);
  stepper.setAcceleration(50);
  
  // Set the speed (steps per second)
  // 200 steps per revolution, 1 revolution per 10 seconds
  stepper.setSpeed(200);

  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Move the motor one step
  stepper.runSpeed();

  // Toggle the LED state
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
