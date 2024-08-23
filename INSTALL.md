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

## Alternative Method: Using esptool.py

If you prefer using the command line, you can use esptool.py to flash the firmware. This method requires Python to be installed on your system.

1. Install esptool.py if you haven't already:
   ```
   pip install esptool
   ```

2. Connect your ESP32 board to your computer via USB.

3. Open a terminal or command prompt and navigate to the directory containing the downloaded firmware file.

4. Run the following command to flash the firmware (replace COM_PORT with your actual COM port, and FIRMWARE_FILE.bin with the actual filename):
   ```
   esptool.py --chip esp32 --port COM_PORT --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x10000 FIRMWARE_FILE.bin
   ```

5. Wait for the process to complete. You should see a success message.

6. Reset your ESP32 board.

This method provides a more flexible and scriptable approach to flashing your Skumfidus device.
