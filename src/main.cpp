#include <Arduino.h>

// Define stepper motor connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32

// Define steps per revolution for 8 microstepping
const int STEPS_PER_REV = 1600; // 200 * 8 (for 8 microstepping)

// Define movement parameters
const float DISTANCE_PER_REV = 4.0; // 4mm per revolution
const float TOTAL_DISTANCE = 100.0; // 100mm (10cm) in each direction
const int TOTAL_STEPS = (TOTAL_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV;

// Define timing variables
unsigned long previousMicros = 0;
const unsigned long stepInterval = 625; // 625 microseconds between steps (1 rev/sec)
int currentStep = 0;
bool movingForward = true;

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

    if (currentStep < TOTAL_STEPS) {
      // Toggle the step pin
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(10); // Short delay for the step pulse
      digitalWrite(STEP_PIN, LOW);

      // Toggle the LED state
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));

      currentStep++;
    } else {
      // Change direction
      movingForward = !movingForward;
      digitalWrite(DIR_PIN, movingForward ? LOW : HIGH);
      currentStep = 0;
      
      // Add a small delay when changing direction
      delay(500);
    }
  }
}
