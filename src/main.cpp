#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <EEPROM.h>
#include "OMDisplay.h"
#include "Timer.h"
#include "ButtonHandler.h"

#define START_BUTTON_PIN 15   // Start button pin
#define HOMING_SWITCH_PIN 16  // Homing switch pin
#define ROTARY_CLK_PIN 17     // Rotary encoder CLK pin
#define ROTARY_DT_PIN 18      // Rotary encoder DT pin
#define ROTARY_SW_PIN 19      // Rotary encoder switch pin

// Initialize ButtonHandler objects
ButtonHandler buttonStart(START_BUTTON_PIN);
ButtonHandler buttonLimitSwitch(HOMING_SWITCH_PIN);
ButtonHandler buttonRotarySwitch(ROTARY_SW_PIN);


// Function to dump switch states
static unsigned long lastDebugPrint = 0;
void dumpDebug() {
    unsigned long currentTime = millis();

    // Debug print every second
    if (currentTime - lastDebugPrint > 1000) {
        Serial.print("Start:");
        Serial.print(buttonStart.getState());
        Serial.print(" Limit:");
        Serial.print(buttonLimitSwitch.getState());
        Serial.print(" Rotary:");
        Serial.println(buttonRotarySwitch.getState());
        lastDebugPrint = currentTime;
    }
}

// Define pin connections
#define STEP_PIN 13
#define DIR_PIN 12
#define STEPPER_ENABLE_PIN 27  // Pin for Stepper Driver ENABLE
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
  RETURNING_TO_START,
  ERROR
};

// Global variable to track system state
volatile SystemState currentSystemState = STARTUP;
SystemState previousSystemState = STARTUP;
bool stateJustChanged = true;

// Flag to control LCD update task
volatile bool lcdUpdateEnabled = false;

// Error message
String errorMessage = "";

// Variable to store the last button state
volatile bool lastButtonState = HIGH;

// Timer variables
Timer timer;
unsigned long timerDuration = 30000; // 30 seconds, adjust as needed

// Define motor states
enum MotorState {
  MOVING,
  CHANGING_DIRECTION
};

// Movement and stepper motor parameters
const int STEPS_PER_REV = 1600;  // 200 * 8 (for 8 microstepping)
const float DISTANCE_PER_REV = 8.0;  // 8mm per revolution (lead of ACME rod)
float TOTAL_DISTANCE = 120.0;  // 30mm in each direction
int TOTAL_STEPS;
float MAX_SPEED = 1600;  // Maintains 2 revolutions per second (16 mm/second)
const float ACCELERATION = 3200.0;  // Adjust for smooth acceleration

// EEPROM addresses
const int EEPROM_TIMER_DURATION_ADDR = 0;
const int EEPROM_TOTAL_DISTANCE_ADDR = 4;
const int EEPROM_MAX_SPEED_ADDR = 8;

// Function to save parameters to EEPROM
void saveParametersToEEPROM() {
  EEPROM.put(EEPROM_TIMER_DURATION_ADDR, timerDuration);
  EEPROM.put(EEPROM_TOTAL_DISTANCE_ADDR, TOTAL_DISTANCE);
  EEPROM.put(EEPROM_MAX_SPEED_ADDR, MAX_SPEED);
  EEPROM.commit();
}

// Function to load parameters from EEPROM
void loadParametersFromEEPROM() {
  EEPROM.get(EEPROM_TIMER_DURATION_ADDR, timerDuration);
  EEPROM.get(EEPROM_TOTAL_DISTANCE_ADDR, TOTAL_DISTANCE);
  EEPROM.get(EEPROM_MAX_SPEED_ADDR, MAX_SPEED);
  
  // Check if values are valid (not NaN or infinity)
  if (isnan(TOTAL_DISTANCE) || isinf(TOTAL_DISTANCE)) TOTAL_DISTANCE = 120.0;
  if (isnan(MAX_SPEED) || isinf(MAX_SPEED)) MAX_SPEED = 1600;
  if (timerDuration == 0xFFFFFFFF) timerDuration = 30000; // Default value if EEPROM is empty
  
  // Update TOTAL_STEPS based on loaded TOTAL_DISTANCE
  TOTAL_STEPS = (TOTAL_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV;
}

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
    if (lcdUpdateEnabled) {
      float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
      updateLCD(distance);
    }
    vTaskDelay(LCD_UPDATE_INTERVAL / portTICK_PERIOD_MS);
  }
}

// Function to enable LCD updates
void enableLCDUpdates() {
  lcdUpdateEnabled = true;
}

// Function to disable LCD updates
void disableLCDUpdates() {
  lcdUpdateEnabled = false;
}

// Global variables for timing
unsigned long stateStartTime = 0;
const unsigned long WELCOME_DURATION = 1000;  // 5 seconds
const unsigned long HOMING_TIMEOUT = 30000;   // 30 seconds

// New function to handle state changes
void changeState(SystemState newState, unsigned long currentTime = 0) {
  previousSystemState = currentSystemState;
  currentSystemState = newState;
  stateStartTime = currentTime == 0 ? millis() : currentTime;
  stateJustChanged = true;
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  // Initialize EEPROM
  EEPROM.begin(512);  // Initialize EEPROM with 512 bytes

  // Load parameters from EEPROM
  loadParametersFromEEPROM();

  // Initialize pins
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(ADDRESSABLE_LED_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEPPER_ENABLE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize ButtonHandler objects
  buttonStart.begin();
  buttonLimitSwitch.begin();
  buttonRotarySwitch.begin();

  // Set STEPPER_ENABLE_PIN to LOW to enable the stepper driver
  digitalWrite(STEPPER_ENABLE_PIN, LOW);

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
  changeState(STARTUP, millis());

  // Save parameters to EEPROM (in case they were not present)
  saveParametersToEEPROM();
}

void handleStartup(unsigned long currentTime) {
  if (stateJustChanged) {
    display.writeDisplay("OrangeMakers", 0, 0);
    display.writeDisplay("Marshmallow 2.0", 1, 0);
    stateJustChanged = false;
  }

  if (currentTime - stateStartTime >= WELCOME_DURATION) {
    changeState(HOMING, currentTime);
  }
}

void handleHoming(unsigned long currentTime) {
  static bool waitingForConfirmation = true;
  static long homingSteps = 0;
  static bool homingStarted = false;
  static bool movingAwayFromSwitch = false;


  if (stateJustChanged) {
    waitingForConfirmation = true;
    homingStarted = false;
    movingAwayFromSwitch = false;
    display.writeDisplay("Start Homing", 0, 0);
    display.writeDisplay("Press knob", 1, 0);
    stateJustChanged = false;
  }

  if (waitingForConfirmation) {
    if (buttonRotarySwitch.isPressed()) {
      waitingForConfirmation = false;
      homingStarted = true;
      stateStartTime = currentTime;  // Reset the start time for homing
      display.writeAlert("Homing begin...", "", 2000);  // Show "Homing..." for 2 seconds
      stepper.setAcceleration(ACCELERATION * 2);  // Set higher acceleration for more instant stop during homing
      homingSteps = HOMING_DIRECTION * 1000000;  // Large number to ensure continuous movement
      stepper.moveTo(homingSteps);
    }
  } else if (homingStarted && !movingAwayFromSwitch) {
    display.writeDisplay("Homing...", 0, 0);
    display.writeDisplay("", 1, 0);

    if (buttonLimitSwitch.getState()) {
      Serial.println("Limit switch triggered!");
      stepper.stop();  // Stop the motor immediately
      stepper.setAcceleration(ACCELERATION);  // Restore original acceleration
      homingSteps = -HOMING_DIRECTION * (5.0 / DISTANCE_PER_REV) * STEPS_PER_REV;  // Move 5mm in opposite direction
      stepper.move(homingSteps);
      movingAwayFromSwitch = true;
    } else if (currentTime - stateStartTime > HOMING_TIMEOUT) {
      // Homing timeout
      changeState(IDLE, currentTime);
      display.writeAlert("Homing failed", "", 2000);
    } else {
      stepper.run();
    }
  } else if (movingAwayFromSwitch) {
    if (stepper.distanceToGo() == 0) {
      // Finished moving away from switch
      stepper.setCurrentPosition(0);
      display.writeAlert("Homing Completed", "", 2000);
      delay(2000);  // Wait for 2 seconds
      changeState(IDLE, currentTime);
    } else {
      stepper.run();
    }
  }
}

void handleIdle() {
  if (stateJustChanged) {
    display.clearDisplay();
    display.writeDisplay("Idle..", 0, 0);
    display.writeDisplay("Press Start", 1, 0);
    stateJustChanged = false;
  }

  if (buttonStart.isPressed()) {
    changeState(RUNNING, millis());
    display.writeAlert("System Started", "", 2000);
    timer.start(timerDuration);
    stepper.moveTo(-HOMING_DIRECTION * TOTAL_STEPS);  // Start moving in opposite direction of homing
  }

  stepper.stop();
}

void handleRunning(unsigned long currentTime) {
  if (stateJustChanged) {
    stateJustChanged = false;
    timer.start(timerDuration);
  }

  bool currentButtonState = digitalRead(START_BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50);  // Simple debounce
    if (digitalRead(START_BUTTON_PIN) == LOW) {
      changeState(RETURNING_TO_START, currentTime);
      display.writeAlert("Abort", "", 2000);
      stepper.moveTo(0);  // Set target to start position
      timer.stop();
      return;
    }
  }

  lastButtonState = currentButtonState;

  if (timer.hasExpired()) {
    changeState(RETURNING_TO_START, currentTime);
    display.writeAlert("Done", "", 2000);
    stepper.moveTo(0);  // Set target to start position
    timer.stop();
    return;
  }

  // Check if homing switch is triggered
  if (buttonLimitSwitch.getState()) {
    changeState(ERROR, currentTime);
    errorMessage = "Endstop trigger";
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
  unsigned long remainingTime = timer.getRemainingTime() / 1000; // Convert to seconds
  float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
  
  display.writeDisplay("Time: " + String(remainingTime) + "s", 0, 0);
  display.writeDisplay("Dist: " + String(distance, 1) + "mm", 1, 0);
}

void handleReturningToStart() {
  if (stateJustChanged) {
    display.clearDisplay();
    stateJustChanged = false;
  }

  if (stepper.distanceToGo() == 0) {
    // We've reached the start position
    changeState(IDLE, millis());
    display.writeAlert("Returned to", "Start Position", 2000);
  } else {
    stepper.run();
    float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
    display.writeDisplay("Returning", 0, 0);
    display.writeDisplay("Dist: " + String(distance, 1) + "mm", 1, 0);
  }
}

void handleError() {
  if (stateJustChanged) {
    // Set STEPPER_ENABLE_PIN to HIGH to disable the stepper driver
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);

    // Always display the error message
    display.writeAlert("Error", errorMessage, 0);  // 0 means display indefinitely

    stateJustChanged = false;
  }
  
  // In ERROR state, we don't do anything else until the device is reset
}

void loop() {
  unsigned long currentTime = millis();

  static unsigned long lastDebugPrint = 0;

  // Debug
  dumpDebug();

  // Update all ButtonHandler objects
  buttonStart.update();
  buttonLimitSwitch.update();
  buttonRotarySwitch.update();

  // Check for homing switch trigger in any state except HOMING, STARTUP, and ERROR
  if (currentSystemState != HOMING && currentSystemState != STARTUP && currentSystemState != ERROR && buttonLimitSwitch.getState()) {
    changeState(ERROR, currentTime);
    errorMessage = "Endstop trigger";
    handleError();  // Immediately handle the error
    return;  // Exit the loop to prevent further state processing
  }

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
    case ERROR:
      handleError();
      break;
  }

  // Reset changed states after handling
  buttonStart.reset();
  buttonLimitSwitch.reset();
  buttonRotarySwitch.reset();
}
