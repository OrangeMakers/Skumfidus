# OrangeMakers DisplayHandler (OMDisplay)

## Overview

The OrangeMakers DisplayHandler (OMDisplay) is a state machine designed to handle all display actions efficiently. It provides a non-blocking, optimized approach to managing LCD updates. OMDisplay is designed to run on a dedicated core of the ESP32, ensuring smooth and consistent display updates without interfering with other system operations.

## Features

1. Non-blocking operation using `millis()` instead of `delay()`
2. Maintains a buffer of the current display content
3. Performs partial updates to minimize I2C communication
4. Supports left and right text alignment
5. Provides an alert function for temporary messages
6. Runs on a dedicated core for optimal performance

## Class Structure

```cpp
class OMDisplay {
public:
    OMDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void writeDisplay(const String& text, uint8_t row, uint8_t startCol, uint8_t endCol = 0, Alignment alignment = Alignment::LEFT);
    void writeAlert(const String& row1, const String& row2, unsigned long duration);
    static void updateTask(void* pvParameters);

private:
    // Private methods and variables
    void update();
};
```

## Usage Examples

### Initializing OMDisplay

```cpp
OMDisplay display(0x27, 16, 2);

void setup() {
    display.begin();
    
    // Create a task for OMDisplay on core 1
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
```

### Updating Display Content

```cpp
// Write left-aligned text
display.writeDisplay("Distance:", 1, 1);

// Write right-aligned text
display.writeDisplay("10.5 mm", 2, 1, 16, Alignment::RIGHT);
```

### Displaying an Alert

```cpp
display.writeAlert("Warning", "Overheating", 3000);
```

## Implementation Details

1. **Dedicated Core**: OMDisplay runs on a dedicated core (typically core 1) of the ESP32, ensuring consistent updates without interference from other tasks.

2. **State Machine**: The `OMDisplay` class implements a state machine with states like IDLE, UPDATING, and ALERT.

3. **Display Buffer**: A 2D array stores the current content of the display, allowing for partial updates.

4. **Update Task**: The `updateTask` method runs continuously on the dedicated core, managing state transitions and performing necessary display updates.

5. **Non-Blocking Operation**: All timing is handled using `millis()` to ensure non-blocking behavior.

6. **Alignment Handling**: The `writeDisplay()` method calculates padding for right-aligned text when necessary.

7. **Alert Functionality**: The `writeAlert()` method temporarily overrides the normal display content for a specified duration.

8. **Thread Safety**: Proper synchronization mechanisms (e.g., mutexes or semaphores) are implemented to ensure thread-safe access to shared resources between cores.

This implementation provides an efficient and flexible way to manage LCD updates while maintaining responsiveness in the main program loop. By running on a dedicated core, OMDisplay ensures smooth and consistent display updates without affecting other system operations.
