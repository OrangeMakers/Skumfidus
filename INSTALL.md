# Installation Instructions for Skumfidus Firmware

## Downloading the Firmware

1. Go to the [Skumfidus Releases page](https://github.com/OrangeMakers/Skumfidus/releases) on GitHub.
2. Find the latest release (e.g., v0.3.0-87).
3. Download the firmware file (e.g., `firmware-v0.3.0-87.bin`).

## Flashing the Firmware

To flash the firmware, you'll need the ESP32 Flash Download Tool. If you don't have it, you can download it from the [Espressif website](https://www.espressif.com/en/support/download/other-tools).

1. Connect your ESP32 board to your computer via USB.
2. Open the ESP32 Flash Download Tool.
3. In the tool:
   - Select the COM port your ESP32 is connected to.
   - Set the baud rate to 115200.
   - Click on the "..." button next to the first empty field and select the downloaded firmware file.
   - Set the address to 0x10000.
   - Check the box next to the firmware file.
4. Click "Start" to begin flashing.
5. Wait for the process to complete. The tool will show "FINISH" when done.
6. Reset your ESP32 board.

Your Skumfidus device should now be running the latest firmware!

Note: Always ensure you're using the latest firmware version for the best performance and features.
