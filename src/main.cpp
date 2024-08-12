#include <Arduino.h>

// Define stepper motor connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32

// Define steps per revolution for 8 microstepping
const int STEPS_PER_REV = 1600; // 200 * 8 (for 8 microstepping)

// Define timing variables
unsigned long previousMicros = 0;
const unsigned long stepInterval = 625; // 625 microseconds between steps (1 rev/sec)

void setup() {
  // Initialize pins as outputs
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Set initial direction (clockwise)
  digitalWrite(DIR_PIN, LOW);
}

void loop() {
  unsigned long currentMicros = micros();

  // Check if it's time to take a step
  if (currentMicros - previousMicros >= stepInterval) {
    previousMicros = currentMicros;

    // Toggle the step pin
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(10); // Short delay for the step pulse
    digitalWrite(STEP_PIN, LOW);

    // Toggle the LED state
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}
