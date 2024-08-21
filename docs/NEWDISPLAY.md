# Display Handling Refactoring Plan

## Current Issues
1. Race conditions in display handling
2. Complexity in managing display updates
3. Blocking LCD operations in the main loop
4. Separate threads for stepper and LCD updates

## Refactoring Goals
1. Simplify display handling logic
2. Eliminate race conditions
3. Improve performance and responsiveness
4. Maintain separation of concerns

## Milestones

### 1. Simplify OMDisplay Class
- Remove queuing mechanism
- Implement a single buffer for current display content
- Simplify update logic

### 2. Implement Thread-Safe Display Updates
- Use a mutex or semaphore to protect shared display data
- Ensure atomic operations for display updates

### 3. Create Dedicated Display Update Thread
- Implement a new thread for handling display updates
- Allow enable/disable functionality for this thread

### 4. Refactor Main Loop and State Handlers
- Remove direct LCD update calls from state handlers
- Implement a centralized display update function

### 5. Implement New Distance Update Mechanism
- Create a separate thread for distance updates
- Allow enable/disable functionality for distance updates

### 6. Update Integration Points
- Modify Settings and other classes to use new display handling methods
- Ensure all parts of the code use the new thread-safe display update mechanism

### 7. Testing and Debugging
- Implement comprehensive tests for new display handling
- Verify thread safety and performance improvements

### 8. Documentation and Code Cleanup
- Update comments and documentation to reflect new display handling approach
- Remove deprecated code and clean up any remaining issues

## Implementation Details

### New MatrixDisplay Class Structure
```cpp
class MatrixDisplay {
public:
    MatrixDisplay(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void updateDisplay(const String& row1, const String& row2);

private:
    LiquidCrystal_I2C _lcd;
    std::mutex _displayMutex;
    std::atomic<bool> _updateThreadRunning;
    char _currentBuffer[2][17];

    void updateThread();
};
```

### Thread-Safe Update Mechanism
```cpp
void MatrixDisplay::updateDisplay(const String& row1, const String& row2) {
    std::lock_guard<std::mutex> lock(_displayMutex);
    strncpy(_currentBuffer[0], row1.c_str(), 16);
    strncpy(_currentBuffer[1], row2.c_str(), 16);
}
```

### Display Update Thread
```cpp
void MatrixDisplay::updateThread() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(_displayMutex);
            _lcd.setCursor(0, 0);
            _lcd.print(_currentBuffer[0]);
            _lcd.setCursor(0, 1);
            _lcd.print(_currentBuffer[1]);
        }
        delay(50);  // Adjust as needed
    }
}
```

This refactoring plan addresses the main issues while providing a clear path forward for implementation. It separates concerns, eliminates race conditions, and provides better control over display updates. The removal of the separate distance update feature simplifies the implementation and makes it more flexible, allowing any content to be displayed through the updateDisplay method.

### Initiating the New Update Feature in Main File

To implement the new display update feature in the main file:

1. Initialize the MatrixDisplay object:
```cpp
MatrixDisplay display(0x27, 16, 2);  // Adjust parameters as needed
```

2. In the setup() function:
```cpp
void setup() {
    // ... other setup code ...
    
    display.begin();
    display.startUpdateThread();
    
    // ... rest of setup code ...
}
```

3. Replace direct LCD update calls with the new updateDisplay method:
```cpp
// Instead of:
// lcd.clear();
// lcd.print("Hello");
// lcd.setCursor(0, 1);
// lcd.print("World");

// Use:
display.updateDisplay("Hello", "World");
```

4. In the loop() function, ensure the display updates are handled:
```cpp
void loop() {
    // ... other loop code ...
    
    // The display updates are now handled in a separate thread,
    // so you don't need to call any update function here.
    
    // ... rest of loop code ...
}
```

6. Before program exit or when switching to a mode that doesn't need display updates:
```cpp
display.stopUpdateThread();
```

By following these steps, you'll integrate the new thread-safe display update mechanism into your main file, ensuring smooth and race-condition-free display updates.
