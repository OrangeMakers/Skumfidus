# Installationsvejledning til Skumfidus Firmware

## Download af Firmware

1. Gå til [Skumfidus Releases-siden](https://github.com/OrangeMakers/Skumfidus/releases) på GitHub.
2. Find den seneste udgivelse (Er markeret med `latest`).
3. Download firmware-filen (f.eks. `firmware.bin`).

## Flashning af Firmware

For at flashe firmwaren skal du bruge esptool.py via kommandolinjen. Denne metode kræver, at Python er installeret på dit system.

1. Download alle firmware-filer (bootloader.bin, partitions.bin, boot_app0.bin, og firmware.bin) fra release-siden og gem dem i samme mappe.

2. Installer esptool.py ved at køre følgende kommando:
   ```
   pip install esptool
   ```

3. Tilslut dit ESP32-board til din computer via USB.

4. Åbn en terminal eller kommandoprompt og naviger til mappen, der indeholder de downloadede firmware-filer.

5. Kør følgende kommando for at flashe firmwaren (erstat COM3 med din faktiske COM-port hvis nødvendigt):

   For Windows (kør i cmd):
   ```
   esptool --chip esp32 --port "COM3" --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB ^
   0x1000 bootloader.bin ^
   0x8000 partitions.bin ^
   0xe000 boot_app0.bin ^
   0x10000 firmware.bin
   ```

   For Windows (kør i PowerShell):
   ```
   esptool --chip esp32 --port "COM3" --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB `
   0x1000 bootloader.bin `
   0x8000 partitions.bin `
   0xe000 boot_app0.bin `
   0x10000 firmware.bin
   ```

   For Unix-systemer:
   ```
   esptool --chip esp32 --port "/dev/ttyUSB0" --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB \
   0x1000 bootloader.bin \
   0x8000 partitions.bin \
   0xe000 boot_app0.bin \
   0x10000 firmware.bin
   ```

6. Vent på at processen er færdig. Du skulle gerne se en succesmeddelelse.

7. Nulstil dit ESP32-board.

Denne metode sikrer, at alle nødvendige firmware-komponenter bliver flashet korrekt til din Skumfidus-enhed.
