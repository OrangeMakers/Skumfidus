# Display Manager

## Overview

The Display Manager is a state machine designed to handle all display actions efficiently. It provides a non-blocking, optimized approach to managing LCD updates.

## Features

1. Non-blocking operation using `millis()` instead of `delay()`
2. Maintains a buffer of the current display content
3. Performs partial updates to minimize I2C communication
4. Supports left and right text alignment
5. Provides an alert function for temporary messages

## Class Structure

```cpp
class LCDManager {
public:
    LCDManager(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void writeDisplay(const String& text, uint8_t row, uint8_t startCol, uint8_t endCol = 0, Alignment alignment = Alignment::LEFT);
    void writeAlert(const String& row1, const String& row2, unsigned long duration);
    void update();

private:
    // Private methods and variables
};
```

## Usage Examples

### Initializing the Display Manager

```cpp
LCDManager lcdManager(0x27, 16, 2);

void setup() {
    lcdManager.begin();
}
```

### Updating Display Content

```cpp
// Write left-aligned text
lcdManager.writeDisplay("Distance:", 1, 1);

// Write right-aligned text
lcdManager.writeDisplay("10.5 mm", 2, 1, 16, Alignment::RIGHT);
```

### Displaying an Alert

```cpp
lcdManager.writeAlert("Warning", "Overheating", 3000);
```

### Regular Updates

```cpp
void loop() {
    // Other code...
    lcdManager.update();
}
```

## Implementation Details

1. **State Machine**: The `LCDManager` class implements a state machine with states like IDLE, UPDATING, and ALERT.

2. **Display Buffer**: A 2D array stores the current content of the display, allowing for partial updates.

3. **Update Function**: The `update()` method is called regularly to manage state transitions and perform necessary display updates.

4. **Non-Blocking Operation**: All timing is handled using `millis()` to ensure non-blocking behavior.

5. **Alignment Handling**: The `writeDisplay()` method calculates padding for right-aligned text when necessary.

6. **Alert Functionality**: The `writeAlert()` method temporarily overrides the normal display content for a specified duration.

This implementation provides an efficient and flexible way to manage LCD updates while maintaining responsiveness in the main program loop.
