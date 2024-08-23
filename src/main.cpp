// #define DEBUG

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ESP32Encoder.h>
#include "MatrixDisplay.h"
#include "Timer.h"
#include "ButtonHandler.h"
#include "Settings.h"
#include "MatrixDisplay.h"
#include "FastLED.h"

#define START_BUTTON_PIN 15   // Start button pin
#define HOMING_SWITCH_PIN 16  // Homing switch pin
#define ROTARY_CLK_PIN 17     // Rotary encoder CLK pin
#define ROTARY_DT_PIN 18      // Rotary encoder DT pin
#define ROTARY_SW_PIN 19      // Rotary encoder switch pin

#define NUM_LEDS 36
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// Initialize ButtonHandler objects
ButtonHandler buttonStart(START_BUTTON_PIN, "Start");
ButtonHandler buttonLimitSwitch(HOMING_SWITCH_PIN, "Limit", false);
ButtonHandler buttonRotarySwitch(ROTARY_SW_PIN, "Rotary");

// Initialize ESP32Encoder
ESP32Encoder encoder;

// Variables for encoder
int32_t lastEncoderValue = 0;
int32_t encoderValue = 0;

// Function to handle encoder changes
void handleEncoderChange(int32_t newValue) {
    #ifdef DEBUG
    if (newValue != lastEncoderValue) {
        Serial.print("Encoder ");
        Serial.print(newValue > lastEncoderValue ? "clockwise" : "anticlockwise");
        Serial.print(" to ");
        Serial.println(newValue);
    }
    #endif
    lastEncoderValue = newValue;
}

// Define pin connections
#define STEP_PIN 13
#define DIR_PIN 12
#define STEPPER_ENABLE_PIN 27  // Pin for Stepper Driver ENABLE
#define BUILTIN_LED_PIN 2  // Built-in LED pin for ESP32
#define ADDRESSABLE_LED_PIN 4  // New pin for Addressable LED
#define RELAY_PIN 14  // Relay control pin

// Define direction constants
#define DIRECTION_HOME 1
#define DIRECTION_RUN 1
#define DIRECTION_ZERO -1

#define HOMING_DISTANCE 125.0 // Distance to move back after hitting the switch (in mm)

#define HOMING_SPEED 1400.0 // Speed for homing movement
#define MOVE_TO_ZERO_SPEED 3000.0 // Speed for moving to zero position after homing

// Define system states
enum SystemState {
  STARTUP,
  HOMING,
  IDLE,
  RUNNING,
  RETURNING_TO_START,
  ERROR,
  SETTINGS_MENU,
  PARKING  // New state
};

// Global variable to track system state
volatile SystemState currentSystemState = STARTUP;
SystemState previousSystemState = STARTUP;
bool stateJustChanged = true;

// Error message
String errorMessage = "";

// Timer variables
Timer timer;

// Define motor states
enum MotorState {
  MOVING,
  CHANGING_DIRECTION
};

// Movement and stepper motor parameters
const int STEPS_PER_REV = 1600;  // 200 * 8 (for 8 microstepping)
const float DISTANCE_PER_REV = 8.0;  // 8mm per revolution (lead of ACME rod)
int TOTAL_STEPS;
const float ACCELERATION = 3200.0;  // Adjust for smooth acceleration

// Define LCD update interval
static unsigned long lastLCDUpdateTime = 0;
const unsigned long LCD_UPDATE_INTERVAL = 250;  // 0.25 second in milliseconds

// Initialize stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Initialize MatrixDisplay
MatrixDisplay display(0x27, 16, 2);

// Initialize Settings
Settings settings(display, encoder);

// Variables for state machine
MotorState currentState = MOVING;
const unsigned long DIRECTION_CHANGE_DELAY = 500; // 500ms delay when changing direction

// Function to initialize and turn on LED strip
void initializeLEDStrip() {
  FastLED.addLeds<LED_TYPE, ADDRESSABLE_LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Orange);
  FastLED.setBrightness(255);  // Set to full brightness
  FastLED.show();
}

// Function to set LED strip to green
void setLEDGreen() {
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
}

// Function to set LED strip to yellow
void setLEDYellow() {
  fill_solid(leds, NUM_LEDS, CRGB::Yellow);
  FastLED.show();
}

// Function to set LED strip to red
void setLEDRed() {
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
}

void startHeater() {
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUILTIN_LED_PIN, HIGH);
  #ifdef DEBUG
  Serial.println("Heater started");
  #endif
}

void stopHeater() {
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUILTIN_LED_PIN, LOW);
  #ifdef DEBUG
  Serial.println("Heater stopped");
  #endif
}

// Function to enter Settings menu
void enterSettingsMenu() {
  settings.enter();  // Enter settings menu
}

// Function to exit Settings menu
void exitSettingsMenu() {
  settings.exit();  // Exit settings menu
}

// Global variables for timing
unsigned long stateStartTime = 0;
const unsigned long WELCOME_DURATION = 1000;  // 5 seconds
const unsigned long HOMING_TIMEOUT = 30000;   // 30 seconds

// Function to convert SystemState to string
const char* getStateName(SystemState state) {
  switch(state) {
    case STARTUP: return "STARTUP";
    case HOMING: return "HOMING";
    case IDLE: return "IDLE";
    case RUNNING: return "RUNNING";
    case RETURNING_TO_START: return "RETURNING_TO_START";
    case ERROR: return "ERROR";
    case SETTINGS_MENU: return "SETTINGS_MENU";
    case PARKING: return "PARKING";
    default: return "UNKNOWN";
  }
}

// New function to handle state changes
void changeState(SystemState newState, unsigned long currentTime = 0) {
  #ifdef DEBUG
  if(newState != previousSystemState){
    Serial.print("State changed from ");
    Serial.print(getStateName(previousSystemState));
    Serial.print(" to ");
    Serial.println(getStateName(newState));
  } else {
    Serial.print("Current state ");
    Serial.println(getStateName(newState));
  }
  #endif
  
  previousSystemState = currentSystemState;
  currentSystemState = newState;
  stateStartTime = currentTime == 0 ? millis() : currentTime;
  stateJustChanged = true;
}

void handleStartup(unsigned long currentTime) {
  if (stateJustChanged) {
    display.updateDisplay("OrangeMakers", "Marshmallow 2.0");
    stateJustChanged = false;
  }

  if (currentTime - stateStartTime >= WELCOME_DURATION) {
    changeState(HOMING, currentTime);
  }
}

static unsigned long lastHomingUpdateTime = 0;

void handleHoming(unsigned long currentTime) {
  static bool waitingForConfirmation = true;
  static long homingSteps = 0;
  static bool homingStarted = false;
  static bool movingAwayFromSwitch = false;

  if (stateJustChanged) {
    waitingForConfirmation = true;
    homingStarted = false;
    movingAwayFromSwitch = false;
    display.updateDisplay("To start homing", "press rotary");
    stateJustChanged = false;
    lastHomingUpdateTime = 0; // Reset the update time when state changes
    setLEDYellow(); // Set LED to yellow when homing begins
  }

  if (waitingForConfirmation) {
    if (buttonRotarySwitch.isPressed()) {
      waitingForConfirmation = false;
      homingStarted = true;
      stateStartTime = currentTime;  // Reset the start time for homing
      digitalWrite(STEPPER_ENABLE_PIN, LOW);  // Enable the stepper motor
      stepper.setMaxSpeed(HOMING_SPEED);
      stepper.setAcceleration(ACCELERATION * 2);  // Set higher acceleration for more instant stop during homing
      homingSteps = DIRECTION_HOME * 1000000;  // Large number to ensure continuous movement
      stepper.moveTo(homingSteps);
      display.updateDisplay("Homing:", "In progress");
    }
  } else if (homingStarted && !movingAwayFromSwitch) {
    if (buttonLimitSwitch.getState()) {
      display.updateDisplay("Homing:", "Triggered");
      stepper.stop();  // Stop as fast as possible: sets new target
      stepper.runToPosition();  // Wait for the stepper to stop
      
      // Wait for 1 second
      delay(1000);
      
      stepper.setMaxSpeed(MOVE_TO_ZERO_SPEED);
      stepper.setAcceleration(ACCELERATION);  // Restore original acceleration
      homingSteps = DIRECTION_ZERO * (HOMING_DISTANCE / DISTANCE_PER_REV) * STEPS_PER_REV;  // Move HOMING_DISTANCE in run direction
      stepper.move(homingSteps);
      movingAwayFromSwitch = true;
      display.updateDisplay("Homing:", "Move to Zero");
    } else if (currentTime - stateStartTime > HOMING_TIMEOUT) {
      // Homing timeout
      errorMessage = "Homing failed";
      changeState(ERROR, currentTime);
      return;
    } else {
      stepper.run();
    }
  } else if (movingAwayFromSwitch) {
    if (stepper.distanceToGo() == 0) {
      // Finished moving away from switch
      stepper.setCurrentPosition(0);
      stepper.setMaxSpeed(settings.getSpeed());  // Restore original max speed
      #ifdef DEBUG
      Serial.println("Homing completed!");
      #endif
      display.updateDisplay("Homing:", "Completed", 2000);
      changeState(IDLE, currentTime);
    } else {
      stepper.run();
    }
  }
}

void handleIdle() {
  const unsigned long LONG_PRESS_DURATION = 5000; // 5 seconds for long press
  const unsigned long SETTINGS_PRESS_DURATION = 1000; // 1 second for settings

  if (stateJustChanged) {
    display.updateDisplay("Idle..", "Press Start");
    stateJustChanged = false;
    setLEDGreen(); // Set LED to green when idle
  }

  static bool startButtonWasPressed = false;
  static unsigned long startPressStartTime = 0;

  if (buttonStart.isPressed()) {
    startButtonWasPressed = true;
    startPressStartTime = millis();
  }

  if (startButtonWasPressed) {
    unsigned long pressDuration = millis() - startPressStartTime;
    if (pressDuration >= LONG_PRESS_DURATION) {
      changeState(PARKING, millis());
      startButtonWasPressed = false;
    } else if (buttonStart.isReleased()) {
      changeState(RUNNING, millis());
      timer.start(settings.getCookTime());
      TOTAL_STEPS = (settings.getTotalDistance() / DISTANCE_PER_REV) * STEPS_PER_REV;
      stepper.moveTo(DIRECTION_RUN * TOTAL_STEPS);
      startButtonWasPressed = false;
    }
  }

  if (buttonRotarySwitch.isPressedForMs() >= SETTINGS_PRESS_DURATION) {
    changeState(SETTINGS_MENU, millis());
    enterSettingsMenu();
  }

  stepper.stop();
}

void handleRunning(unsigned long currentTime) {

  if (stateJustChanged) {
    stateJustChanged = false;
    display.updateDisplay("Cooking", "Started");
    timer.start(settings.getCookTime());
    currentState = MOVING;  // Ensure we start in the MOVING state
    TOTAL_STEPS = (settings.getTotalDistance() / DISTANCE_PER_REV) * STEPS_PER_REV;
    stepper.setMaxSpeed(settings.getSpeed());  // Set the correct max speed
    stepper.moveTo(DIRECTION_RUN * TOTAL_STEPS);  // Set initial movement direction
    lastLCDUpdateTime = 0; // Force an immediate update
    startHeater(); // Start the heater when entering the running state
    setLEDRed(); // Set LED to red when running
  }

  if (buttonStart.isPressed()) {
    stopHeater(); // Stop the heater
    changeState(RETURNING_TO_START, currentTime);
    display.updateDisplay("Cooking", "Aborted");
    stepper.moveTo(0);  // Set target to start position
    timer.stop();
    return;
  }

  if (timer.hasExpired()) {
    stopHeater(); // Stop the heater
    changeState(RETURNING_TO_START, currentTime);
    display.updateDisplay("Cooking", "Done");
    stepper.moveTo(0);  // Set target to start position
    timer.stop();
    return;
  }

  // Check if homing switch is triggered
  if (buttonLimitSwitch.getState()) {
    stopHeater(); // Stop the heater
    changeState(ERROR, currentTime);
    errorMessage = "Endstop trigger";
    return;
  }

  switch (currentState) {
    case MOVING:
      if (stepper.distanceToGo() == 0) {
        // Change direction when reaching either end
        TOTAL_STEPS = (settings.getTotalDistance() / DISTANCE_PER_REV) * STEPS_PER_REV;
        stepper.moveTo(stepper.currentPosition() == 0 ? DIRECTION_RUN * TOTAL_STEPS : (stepper.currentPosition() == DIRECTION_RUN * TOTAL_STEPS ? 0 : DIRECTION_RUN * TOTAL_STEPS));
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

  // Update LCD with remaining time and distance at specified interval
  if (currentTime - lastLCDUpdateTime >= LCD_UPDATE_INTERVAL) {
    unsigned long remainingTime = timer.getRemainingTime() / 1000; // Convert to seconds
    float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
    
    String timeStr = "Time: " + String(remainingTime) + "s";
    String distStr = "Dist: " + String(distance, 1) + "mm";
    display.updateDisplay(timeStr, distStr);
    
    lastLCDUpdateTime = currentTime;
  }
}

void handleReturningToStart() {
  unsigned long currentTime = millis();

  if (stateJustChanged) {
    stateJustChanged = false;
    stepper.setMaxSpeed(settings.getSpeed());  // Set the correct max speed
    lastLCDUpdateTime = 0; // Force an immediate update
    stopHeater(); // Stop the heater when returning to start
  }

  if (stepper.distanceToGo() == 0) {
    // We've reached the start position
    changeState(IDLE, currentTime);
    display.updateDisplay("Returned to", "Start Position");
  } else {
    stepper.run();

    if (currentTime - lastLCDUpdateTime >= LCD_UPDATE_INTERVAL) {
      float distance = abs(stepper.currentPosition() * DISTANCE_PER_REV / STEPS_PER_REV);
      String returnStr = "Returning";
      String distStr = "Dist: " + String(distance, 1) + "mm";
      display.updateDisplay(returnStr, distStr);
      lastLCDUpdateTime = currentTime;
    }
  }
}

void handleError() {
  if (stateJustChanged) {
    // Set STEPPER_ENABLE_PIN to HIGH to disable the stepper driver
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
    stopHeater(); // Stop the heater in case of an error
    stateJustChanged = false;
  }
  
  // Always display the error message
  display.updateDisplay("Error", errorMessage);

  // In ERROR state, we don't do anything else until the device is reset
}

#ifdef DEBUG
// Function to dump switch states and encoder value
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
        Serial.print(buttonRotarySwitch.getState());
        Serial.print(" Encoder:");
        Serial.print(encoderValue);
        Serial.print(" Direction:");
        Serial.print(encoderValue > lastEncoderValue ? "CW" : (encoderValue < lastEncoderValue ? "CCW" : "No change"));
        Serial.print(" CookTime:");
        Serial.print(settings.getCookTime());
        Serial.print(" Speed:");
        Serial.print(settings.getSpeed());
        Serial.print(" TotalDistance:");
        Serial.println(settings.getTotalDistance());
        lastDebugPrint = currentTime;
    }
}
#endif

// Function to display current settings
#ifdef DEBUG
void displayCurrentSettings() {
  Serial.println("Current Settings:");
  Serial.print("Cook Time: ");
  Serial.print(settings.getCookTime());
  Serial.println(" ms");
  Serial.print("Total Distance: ");
  Serial.print(settings.getTotalDistance());
  Serial.println(" mm");
  Serial.print("Speed: ");
  Serial.print(settings.getSpeed());
  Serial.println(" steps/second");
}
#endif

void handleParking() {
  static bool parkingStarted = false;

  if (stateJustChanged) {
    stateJustChanged = false;
    parkingStarted = false;
    display.updateDisplay("Parking", "Please wait");
    digitalWrite(STEPPER_ENABLE_PIN, LOW);  // Enable the stepper motor
    stepper.setMaxSpeed(settings.getSpeed());
    stepper.setAcceleration(ACCELERATION);
    float parkDistance = 120.0;  // 120mm parking distance
    long parkSteps = (parkDistance / DISTANCE_PER_REV) * STEPS_PER_REV;
    stepper.moveTo(DIRECTION_HOME * parkSteps);
    parkingStarted = true;
  }

  if (parkingStarted) {
    if (stepper.distanceToGo() == 0) {
      // Parking completed
      digitalWrite(STEPPER_ENABLE_PIN, HIGH);  // Disable the stepper motor
      display.updateDisplay("Please turn off", "The power");
      while (true) {
        // Infinite loop to stop all processing
        delay(1000);
      }
    } else {
      stepper.run();
    }
  }
}

void setup() {
  settings.loadSettingsFromPreferences();

  // Initialize LED strip
  initializeLEDStrip();

  // Init if debug
  #ifdef DEBUG
  Serial.begin(115200);  // Initialize serial communication
  
  // Print initial settings
  displayCurrentSettings();
  #endif

  // Initialize pins
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  pinMode(ADDRESSABLE_LED_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEPPER_ENABLE_PIN, OUTPUT);
  digitalWrite(STEPPER_ENABLE_PIN, HIGH);  // Set STEPPER_ENABLE_PIN to HIGH by default
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Set RELAY_PIN to LOW by default

  // Rotary Encoder
  pinMode(ROTARY_CLK_PIN, INPUT_PULLUP);
  pinMode(ROTARY_DT_PIN, INPUT_PULLUP);
  pinMode(ROTARY_SW_PIN, INPUT_PULLUP);

  // Initialize ButtonHandler objects
  buttonStart.begin();
  buttonLimitSwitch.begin();
  buttonRotarySwitch.begin();

  // Initialize ESP32Encoder
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachHalfQuad(ROTARY_CLK_PIN, ROTARY_DT_PIN);
  encoder.setCount(0);

  // Initialize LCD
  display.begin();

  // Configure stepper
  stepper.setMaxSpeed(settings.getSpeed());
  stepper.setAcceleration(ACCELERATION);
  stepper.moveTo(0);  // Start at home position

  // Initialize and start MatrixDisplay update thread
  display.begin();
  display.startUpdateThread();

  // Initialize state
  changeState(STARTUP, millis());
}

void loop() {
  unsigned long currentTime = millis();


  // Debug
  #ifdef DEBUG
  static unsigned long lastDebugPrint = 0;
  dumpDebug();
  #endif

  // Update all ButtonHandler objects
  buttonStart.update();
  buttonLimitSwitch.update();
  buttonRotarySwitch.update();

  // Read encoder value
  encoderValue = encoder.getCount();

  // Check if encoder value has changed
  if (encoderValue != lastEncoderValue) {
    // Handle encoder change
    int32_t change = encoderValue - lastEncoderValue;
    // You can use 'change' to update menu selection or modify values
    // For example: menuIndex = (menuIndex + change) % MENU_ITEMS;
    handleEncoderChange(encoderValue);
  }

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
    case SETTINGS_MENU:
      if (!settings.isDone()) {
        settings.update();
      }
      if (settings.isDone()) {
        changeState(IDLE, currentTime);
      }
      break;
    case PARKING:
      handleParking();
      break;
  }

  // Reset changed states after handling
  buttonStart.reset();
  buttonLimitSwitch.reset();
  buttonRotarySwitch.reset();
}
