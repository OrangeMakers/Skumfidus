#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Define pin connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32
#define RELAY_PIN 14  // Relay control pin

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

// Task to update LCD
void lcdUpdateTask(void * parameter) {
  for(;;) {
    float distance = stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV;
    updateLCD(distance);
    vTaskDelay(LCD_UPDATE_INTERVAL / portTICK_PERIOD_MS);
  }
}

// Task to control relay
void relayControlTask(void * parameter) {
  for(;;) {
    digitalWrite(RELAY_PIN, HIGH);  // Turn on relay
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Wait for 10 seconds
    digitalWrite(RELAY_PIN, LOW);   // Turn off relay
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Wait for 10 seconds
  }
}

void setup() {
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  updateLCD(0);

  // Configure stepper
  stepper.setMaxSpeed(3200);  // 2 revolutions per second
  stepper.setAcceleration(1600);  // Adjust for smooth acceleration
  stepper.moveTo(TOTAL_STEPS);

  // Create LCD update task
  xTaskCreate(
    lcdUpdateTask,    // Function that should be called
    "LCD Update",     // Name of the task (for debugging)
    2048,             // Stack size (bytes)
    NULL,             // Parameter to pass
    1,                // Task priority
    NULL              // Task handle
  );

  // Create relay control task
  xTaskCreate(
    relayControlTask, // Function that should be called
    "Relay Control",  // Name of the task (for debugging)
    1024,             // Stack size (bytes)
    NULL,             // Parameter to pass
    1,                // Task priority
    NULL              // Task handle
  );
}

void loop() {
  // Motor control logic
  if (stepper.distanceToGo() == 0) {
    // Change direction when reaching either end
    stepper.moveTo(stepper.currentPosition() == 0 ? TOTAL_STEPS : 0);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Toggle LED when changing direction
    delay(500);  // Small delay when changing direction
  }

  stepper.run();
}
