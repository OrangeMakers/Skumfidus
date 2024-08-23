# Check if Python is installed
try {
    python --version | Out-Null
}
catch {
    Write-Host "Python is not installed or not in PATH."
    Write-Host "Please visit https://github.com/OrangeMakers/Skumfidus/blob/main/INSTALL.md for installation instructions."
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if esptool is installed
try {
    esptool version | Out-Null
}
catch {
    Write-Host "esptool is not installed."
    Write-Host "Please visit https://github.com/OrangeMakers/Skumfidus/blob/main/INSTALL.md for installation instructions."
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if firmware files exist
$files = @("bootloader.bin", "partitions.bin", "boot_app0.bin", "firmware.bin")
foreach ($file in $files) {
    if (-not (Test-Path $file)) {
        Write-Host "$file is missing from the current directory."
        Write-Host "Please ensure all firmware files are in the same folder as this script."
        Read-Host "Press Enter to exit"
        exit 1
    }
}

# Set COM port
$COM_PORT = Read-Host "Enter the COM port (e.g., COM3)"

# Flash firmware
Write-Host "Flashing firmware..."
$command = "esptool --chip esp32 --port $COM_PORT --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB " +
           "0x1000 bootloader.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 firmware.bin"

try {
    Invoke-Expression $command
}
catch {
    Write-Host "An error occurred while flashing the firmware."
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host "Firmware flashed successfully!"
Read-Host "Press Enter to exit"
