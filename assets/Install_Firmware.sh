#!/bin/bash

# Check if Python is installed
if ! command -v python &> /dev/null
then
    echo "Python is not installed or not in PATH."
    echo "Please visit https://github.com/OrangeMakers/Skumfidus/blob/main/INSTALL.md for installation instructions."
    read -p "Press Enter to exit"
    exit 1
fi

# Check if esptool is installed
if ! command -v esptool &> /dev/null
then
    echo "esptool is not installed."
    echo "Please visit https://github.com/OrangeMakers/Skumfidus/blob/main/INSTALL.md for installation instructions."
    read -p "Press Enter to exit"
    exit 1
fi

# Check if firmware files exist
files=("bootloader.bin" "partitions.bin" "boot_app0.bin" "firmware.bin")
for file in "${files[@]}"
do
    if [ ! -f "$file" ]; then
        echo "$file is missing from the current directory."
        echo "Please ensure all firmware files are in the same folder as this script."
        read -p "Press Enter to exit"
        exit 1
    fi
done

# Set COM port
read -p "Enter the serial port (e.g., /dev/ttyUSB0): " SERIAL_PORT

# Flash firmware
echo "Flashing firmware..."
esptool --chip esp32 --port "$SERIAL_PORT" --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB \
0x1000 bootloader.bin \
0x8000 partitions.bin \
0xe000 boot_app0.bin \
0x10000 firmware.bin

if [ $? -ne 0 ]; then
    echo "An error occurred while flashing the firmware."
    read -p "Press Enter to exit"
    exit 1
fi

echo "Firmware flashed successfully!"
read -p "Press Enter to exit"
