#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "OMDisplay.h"

#define START_BUTTON_PIN 15   // Start button pin
#define HOMING_SWITCH_PIN 16  // Homing switch pin
#define ROTARY_CLK_PIN 17     // Rotary encoder CLK pin
#define ROTARY_DT_PIN 18      // Rotary encoder DT pin
#define ROTARY_SW_PIN 19      // Rotary encoder switch pin

// Define pin connections
#define STEP_PIN 13
#define DIR_PIN 12
#define ENABLE_PIN 27  // New pin for Stepper Driver ENABLE
#define BUILTIN_LED_PIN 2  // Built-in LED pin for ESP32
#define ADDRESSABLE_LED_PIN 4  // New pin for Addressable LED
#define RELAY_PIN 14  // Relay control pin

// Define homing direction (1 for positive, -1 for negative)
#define HOMING_DIRECTION 1

// Define system states
enum SystemState {
  STARTUP,
  HOMING,
  IDLE,
  RUNNING,
  RETURNING_TO_START
};

// Global variable to track system state
volatile SystemState currentSystemState = STARTUP;

// Variable to store the last button state
volatile bool lastButtonState = HIGH;

// Timer variables
unsigned long timerStartTime = 0;
const unsigned long timerDuration = 30000; // 60 seconds, adjust as needed
bool timerRunning = false;

// Define motor states
enum MotorState {
  MOVING,
  CHANGING_DIRECTION
};

// Movement and stepper motor parameters
const int STEPS_PER_REV = 1600;  // 200 * 8 (for 8 microstepping)
const float DISTANCE_PER_REV = 8.0;  // 8mm per revolution (lead of ACME rod)
const float TOTAL_DISTANCE = 120.0;  // 30mm in each direction
const int TOTAL_STEPS = (TOTAL_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV;
const float MAX_SPEED = 1600;  // Maintains 2 revolutions per second (16 mm/second)
const float ACCELERATION = 3200.0;  // Adjust for smooth acceleration

// Define LCD update interval
const unsigned long LCD_UPDATE_INTERVAL = 250;  // 0.25 second in milliseconds

// Initialize stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Initialize OMDisplay
OMDisplay display(0x27, 16, 2);

// Variables for state machine
MotorState currentState = MOVING;
const unsigned long DIRECTION_CHANGE_DELAY = 500; // 500ms delay when changing direction

// Function to update LCD display
void updateLCD(float distance) {
  display.writeDisplay("Distance:", 0, 0);
  String distanceStr = String(distance, 1) + " mm";
  display.writeDisplay(distanceStr, 1, 0, 10, Alignment::LEFT);
  // Placeholder for future implementation
  display.writeDisplay("", 1, 11, 16, Alignment::RIGHT);
}

// Task to update LCD
void lcdUpdateTask(void * parameter) {
  for(;;) {
    if (currentSystemState == RUNNING) {
      float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
      updateLCD(distance);
    }
    vTaskDelay(LCD_UPDATE_INTERVAL / portTICK_PERIOD_MS);
  }
}

// Global variables for timing
unsigned long stateStartTime = 0;
const unsigned long WELCOME_DURATION = 1000;  // 5 seconds
const unsigned long HOMING_TIMEOUT = 30000;   // 30 seconds

void setup() {
  // Initialize pins
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(ADDRESSABLE_LED_PIN, OUTPUT);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);  // Start button with internal pull-up
  pinMode(HOMING_SWITCH_PIN, INPUT_PULLUP); // Homing switch with internal pull-up
  pinMode(ROTARY_CLK_PIN, INPUT_PULLUP);    // Rotary encoder CLK with internal pull-up
  pinMode(ROTARY_DT_PIN, INPUT_PULLUP);     // Rotary encoder DT with internal pull-up
  pinMode(ROTARY_SW_PIN, INPUT_PULLUP);     // Rotary encoder switch with internal pull-up
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Set ENABLE_PIN to LOW to enable the stepper driver
  digitalWrite(ENABLE_PIN, LOW);

  // Initialize LCD
  display.begin();

  // Configure stepper
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.moveTo(0);  // Start at home position

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

  // Create LCD update task
  xTaskCreate(
    lcdUpdateTask,           // Task function
    "LCD Update",            // Task name
    2048,                    // Stack size (bytes)
    NULL,                    // Parameter to pass
    1,                       // Task priority
    NULL                     // Task handle
  );

  // Initialize state
  currentSystemState = STARTUP;
  stateStartTime = millis();
}

void handleStartup(unsigned long currentTime) {
  if (currentTime - stateStartTime < WELCOME_DURATION) {
    // Display welcome message
    display.writeDisplay("OrangeMakers", 0, 0);
    display.writeDisplay("Marshmallow 2.0", 1, 0);
  } else {
    // Transition to HOMING state
    currentSystemState = HOMING;
    stateStartTime = currentTime;
  }
}

void handleHoming(unsigned long currentTime) {
  static bool waitingForConfirmation = true;
  static bool homingSwitchTriggered = false;
  static long homingSteps = 0;
  static bool homingStarted = false;

  if (waitingForConfirmation) {
    display.writeDisplay("Start Homing", 0, 0);
    display.writeDisplay("Press knob", 1, 0);

    if (digitalRead(ROTARY_SW_PIN) == LOW) {
      delay(50);  // Simple debounce
      if (digitalRead(ROTARY_SW_PIN) == LOW) {
        waitingForConfirmation = false;
        homingStarted = true;
        stateStartTime = currentTime;  // Reset the start time for homing
        display.writeAlert("Homing begin...", "", 2000);  // Show "Homing..." for 2 seconds
        stepper.setAcceleration(ACCELERATION * 2);  // Set higher acceleration for more instant stop during homing
        homingSteps = HOMING_DIRECTION * 1000000;  // Large number to ensure continuous movement
        stepper.moveTo(homingSteps);
      }
    }
  } else if (!homingSwitchTriggered) {
    if (homingStarted) {
      display.writeDisplay("Homing...", 0, 0);
      display.writeDisplay("", 1, 0);
    }
    if (digitalRead(HOMING_SWITCH_PIN) == HIGH) {  // Homing switch triggered
      homingSwitchTriggered = true;
      stepper.stop();  // Stop the motor immediately
      stepper.setAcceleration(ACCELERATION);  // Restore original acceleration
      homingSteps = -HOMING_DIRECTION * (5.0 / DISTANCE_PER_REV) * STEPS_PER_REV;  // Move 5mm in opposite direction
      stepper.move(homingSteps);
    } else if (currentTime - stateStartTime > HOMING_TIMEOUT) {
      // Homing timeout
      currentSystemState = IDLE;
      display.writeAlert("Homing failed", "", 2000);
      waitingForConfirmation = true;  // Reset for next homing
      homingSwitchTriggered = false;
      homingStarted = false;
    } else {
      stepper.run();
    }
  } else {
    if (stepper.distanceToGo() == 0) {
      // Finished moving away from switch
      stepper.setCurrentPosition(0);
      display.writeAlert("Homing Completed", "", 2000);
      delay(2000);  // Wait for 2 seconds
      currentSystemState = IDLE;
      waitingForConfirmation = true;  // Reset for next homing
      homingSwitchTriggered = false;
      homingStarted = false;
    } else {
      stepper.run();
    }
  }
}

void handleIdle() {
  static bool enteredIdle = true;

  if (enteredIdle) {
    display.clearDisplay();
    enteredIdle = false;
  }

  bool currentButtonState = digitalRead(START_BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50);  // Simple debounce
    if (digitalRead(START_BUTTON_PIN) == LOW) {
      currentSystemState = RUNNING;
      display.writeAlert("System Started", "", 2000);
      timerStartTime = millis();
      timerRunning = true;
      enteredIdle = true;  // Reset for next time we enter idle
      stepper.moveTo(-HOMING_DIRECTION * TOTAL_STEPS);  // Start moving in opposite direction of homing
    }
  }

  lastButtonState = currentButtonState;
  stepper.stop();
  display.writeDisplay("Idle..", 0, 0);
  display.writeDisplay("Press Start", 1, 0);
}

void handleRunning(unsigned long currentTime) {
  bool currentButtonState = digitalRead(START_BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50);  // Simple debounce
    if (digitalRead(START_BUTTON_PIN) == LOW) {
      currentSystemState = RETURNING_TO_START;
      display.writeAlert("Abort", "", 2000);
      stepper.moveTo(0);  // Set target to start position
      timerRunning = false;
      return;
    }
  }

  lastButtonState = currentButtonState;

  if (timerRunning && (currentTime - timerStartTime >= timerDuration)) {
    currentSystemState = RETURNING_TO_START;
    display.writeAlert("Done", "", 2000);
    stepper.moveTo(0);  // Set target to start position
    timerRunning = false;
    return;
  }

  switch (currentState) {
    case MOVING:
      if (stepper.distanceToGo() == 0) {
        // Change direction when reaching either end
        stepper.moveTo(stepper.currentPosition() == 0 ? -HOMING_DIRECTION * TOTAL_STEPS : (stepper.currentPosition() == -HOMING_DIRECTION * TOTAL_STEPS ? 0 : -HOMING_DIRECTION * TOTAL_STEPS));
        digitalWrite(BUILTIN_LED_PIN, !digitalRead(BUILTIN_LED_PIN));  // Toggle LED when changing direction
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

  // Update LCD with remaining time and distance
  unsigned long elapsedTime = (currentTime - timerStartTime) / 1000; // Convert to seconds
  unsigned long remainingTime = (timerDuration / 1000) - elapsedTime;
  float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
  
  display.writeDisplay("Time: " + String(remainingTime) + "s", 0, 0);
  display.writeDisplay("Dist: " + String(distance, 1) + "mm", 1, 0);
}

void handleReturningToStart() {
  static bool firstEntry = true;

  if (firstEntry) {
    display.clearDisplay();
    firstEntry = false;
  }

  if (stepper.distanceToGo() == 0) {
    // We've reached the start position
    currentSystemState = IDLE;
    display.writeAlert("Returned to", "Start Position", 2000);
    firstEntry = true;  // Reset for next time
  } else {
    stepper.run();
    float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
    display.writeDisplay("Returning", 0, 0);
    display.writeDisplay("Dist: " + String(distance, 1) + "mm", 1, 0);
  }
}

void loop() {
  unsigned long currentTime = millis();

  switch (currentSystemState) {
    case STARTUP:
      handleStartup(currentTime);
      break;
    case HOMING:
      handleHoming(currentTime);
      break;
    case IDLE:
      handleIdle();
      break;
    case RUNNING:
      handleRunning(currentTime);
      break;
    case RETURNING_TO_START:
      handleReturningToStart();
      break;
  }
}
