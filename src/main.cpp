#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

// Define pin connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32

// Define stepper motor parameters
const int STEPS_PER_REV = 1600;  // 200 * 8 (for 8 microstepping)
const float DISTANCE_PER_REV = 4.0;  // 4mm per revolution
const float TOTAL_DISTANCE = 30.0;  // 30mm in each direction
const int TOTAL_STEPS = (TOTAL_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV;

// Define LCD update interval
const unsigned long LCD_UPDATE_INTERVAL = 1000;  // 1 second in milliseconds

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

// Initialize stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Function to update LCD display
void updateLCD(float distance) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance, 1);
  lcd.print(" mm");
}

void setup() {
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  updateLCD(0);

  // Configure stepper
  stepper.setMaxSpeed(3200);  // 2 revolutions per second
  stepper.setAcceleration(1600);  // Adjust for smooth acceleration
  stepper.moveTo(TOTAL_STEPS);
}

void loop() {
  static unsigned long lastLCDUpdate = 0;
  unsigned long currentMillis = millis();

  // Motor control logic
  if (stepper.distanceToGo() == 0) {
    // Change direction when reaching either end
    stepper.moveTo(stepper.currentPosition() == 0 ? TOTAL_STEPS : 0);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Toggle LED when changing direction
    delay(500);  // Small delay when changing direction
  }

  stepper.run();

  // LCD update logic
  if (currentMillis - lastLCDUpdate >= LCD_UPDATE_INTERVAL) {
    lastLCDUpdate = currentMillis;
    float distance = stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV;
    updateLCD(distance);
  }
}
