#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

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
unsigned long lastLCDUpdate = 0;
const unsigned long lcdUpdateInterval = 1000000; // 1 second in microseconds

// LCD update variables
bool isUpdatingLCD = false;
int lcdUpdateStep = 0;

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

// Initialize stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Function to update LCD display (non-blocking)
void updateLCD(float distance) {
  if (!isUpdatingLCD) {
    isUpdatingLCD = true;
    lcdUpdateStep = 0;
  }

  switch (lcdUpdateStep) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Distance:");
      lcdUpdateStep++;
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear the second line
      lcd.setCursor(0, 1);
      lcd.print(distance, 1);
      lcd.print(" mm");
      lcdUpdateStep++;
      break;
    case 2:
      isUpdatingLCD = false;
      break;
  }
}

void setup() {
  // Initialize pins as outputs
  pinMode(LED_PIN, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  updateLCD(0);

  // Configure stepper
  stepper.setMaxSpeed(1600); // 1 revolution per second
  stepper.setAcceleration(800); // Adjust this value for smooth acceleration
  stepper.moveTo(TOTAL_STEPS);
}

void loop() {
  unsigned long currentMicros = micros();

  // Motor control logic
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(-stepper.currentPosition());
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED when changing direction
    delay(500); // Small delay when changing direction
  }

  stepper.run();

  // LCD update logic
  if (currentMicros - lastLCDUpdate >= lcdUpdateInterval || isUpdatingLCD) {
    if (!isUpdatingLCD) {
      lastLCDUpdate = currentMicros;
    }
    float distance = abs(stepper.currentPosition()) * DISTANCE_PER_REV / STEPS_PER_REV;
    updateLCD(distance);
  }
}
