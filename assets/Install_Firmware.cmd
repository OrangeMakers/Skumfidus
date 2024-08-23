@echo off
setlocal enabledelayedexpansion

:: Check if Python is installed
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed or not in PATH.
    echo Please visit https://github.com/OrangeMakers/Skumfidus/blob/main/INSTALL.md for installation instructions.
    pause
    exit /b 1
)

:: Check if esptool is installed
esptool version >nul 2>&1
if %errorlevel% neq 0 (
    echo esptool is not installed.
    echo Please visit https://github.com/OrangeMakers/Skumfidus/blob/main/INSTALL.md for installation instructions.
    pause
    exit /b 1
)

:: Check if firmware files exist
set "files=bootloader.bin partitions.bin boot_app0.bin firmware.bin"
for %%f in (%files%) do (
    if not exist "%%f" (
        echo %%f is missing from the current directory.
        echo Please ensure all firmware files are in the same folder as this script.
        pause
        exit /b 1
    )
)

:: Set COM port or use auto-detection
set /p COM_PORT="Enter the COM port (e.g., COM3) or press Enter for auto-detection: "

:: Flash firmware
echo Flashing firmware...
if not defined COM_PORT (
    esptool --chip esp32 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ^
) else (
    esptool --chip esp32 --port %COM_PORT% --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ^
)
0x1000 bootloader.bin ^
0x8000 partitions.bin ^
0xe000 boot_app0.bin ^
0x10000 firmware.bin

if %errorlevel% neq 0 (
    echo An error occurred while flashing the firmware.
    pause
    exit /b 1
)

echo Firmware flashed successfully!
pause
