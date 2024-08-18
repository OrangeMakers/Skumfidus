#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "OMDisplay.h"

#define START_BUTTON_PIN 15  // Start button pin

// Define motor states
enum MotorState {
  MOVING,
  CHANGING_DIRECTION
};

// Define pin connections
#define STEP_PIN 13
#define DIR_PIN 12
#define LED_PIN 2  // Built-in LED pin for ESP32
#define RELAY_PIN 14  // Relay control pin

// Define system states
enum SystemState {
  IDLE,
  RUNNING
};

// Global variable to track system state
volatile SystemState currentSystemState = IDLE;

// Variable to store the last button state
volatile bool lastButtonState = HIGH;

// Movement and stepper motor parameters
const int STEPS_PER_REV = 1600;  // 200 * 8 (for 8 microstepping)
const float DISTANCE_PER_REV = 8.0;  // 8mm per revolution (lead of ACME rod)
const float TOTAL_DISTANCE = 30.0;  // 30mm in each direction
const int TOTAL_STEPS = (TOTAL_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV;
const float MAX_SPEED = 3200;  // Maintains 2 revolutions per second (16 mm/second)
const float ACCELERATION = 1600;  // Adjust for smooth acceleration

// Define LCD update interval
const unsigned long LCD_UPDATE_INTERVAL = 250;  // 0.25 second in milliseconds

// Initialize LCD
// LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

// Global variable to track relay state
volatile bool relayState = false;

// Initialize stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Initialize OMDisplay
OMDisplay display(0x27, 16, 2);

// Variables for state machine
MotorState currentState = MOVING;
unsigned long stateStartTime = 0;
const unsigned long DIRECTION_CHANGE_DELAY = 500; // 500ms delay when changing direction

// Function to update LCD display
void updateLCD(float distance) {
  display.writeDisplay("Distance:", 0, 0);
  String distanceStr = String(distance, 1) + " mm";
  display.writeDisplay(distanceStr, 1, 0, 10, Alignment::LEFT);
  display.writeDisplay(relayState ? "On" : "Off", 1, 11, 16, Alignment::RIGHT);
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
    relayState = true;
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Wait for 10 seconds
    digitalWrite(RELAY_PIN, LOW);   // Turn off relay
    relayState = false;
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Wait for 10 seconds
  }
}

void setup() {
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);  // Initialize start button pin with internal pull-up

  // Initialize LCD
  display.begin();

  // Display welcome message
  display.writeDisplay("OrangeMakers", 0, 0);
  display.writeDisplay("Marsh Mellow 2.0", 1, 0);
  delay(5000);  // Display for 5 seconds

  // Clear LCD and initialize display
  display.clearDisplay();
  updateLCD(0);

  // Configure stepper
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.moveTo(TOTAL_STEPS);

  // Create LCD update task
  // xTaskCreate(
  //   lcdUpdateTask,    // Function that should be called
  //   "LCD Update",     // Name of the task (for debugging)
  //   2048,             // Stack size (bytes)
  //   NULL,             // Parameter to pass
  //   1,                // Task priority
  //   NULL              // Task handle
  // );

  // Create relay control task
  xTaskCreate(
    relayControlTask, // Function that should be called
    "Relay Control",  // Name of the task (for debugging)
    1024,             // Stack size (bytes)
    NULL,             // Parameter to pass
    1,                // Task priority
    NULL              // Task handle
  );

  // Create OMDisplay update task
  xTaskCreatePinnedToCore(
    OMDisplay::updateTask,   // Task function
    "OMDisplay",             // Task name
    4096,                    // Stack size (bytes)
    (void*)&display,         // Parameter to pass
    1,                       // Task priority
    NULL,                    // Task handle
    1                        // Core where the task should run
  );
}

void loop() {
  unsigned long currentTime = millis();
  bool currentButtonState = digitalRead(START_BUTTON_PIN);

  // Check for button press (transition from HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50);  // Simple debounce
    if (digitalRead(START_BUTTON_PIN) == LOW) {
      if (currentSystemState == IDLE) {
        currentSystemState = RUNNING;
        display.writeAlert("System Started", "", 2000);
        updateLCD(0);
      } else {
        currentSystemState = IDLE;
        display.writeAlert("System Idle", "", 2000);
        updateLCD(0);
        // Reset stepper position
        stepper.setCurrentPosition(0);
        stepper.moveTo(0);
      }
    }
  }

  // Update last button state
  lastButtonState = currentButtonState;

  if (currentSystemState == RUNNING) {
    switch (currentState) {
      case MOVING:
        if (stepper.distanceToGo() == 0) {
          // Change direction when reaching either end
          stepper.moveTo(stepper.currentPosition() == 0 ? TOTAL_STEPS : 0);
          digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Toggle LED when changing direction
          currentState = CHANGING_DIRECTION;
          stateStartTime = currentTime;
        } else {
          stepper.run();
        }
        break;

      case CHANGING_DIRECTION:
        if (currentTime - stateStartTime >= DIRECTION_CHANGE_DELAY) {
          currentState = MOVING;
        }
        break;
    }
  } else {
    // System is idle, ensure motor is stopped
    stepper.stop();
  }
}
