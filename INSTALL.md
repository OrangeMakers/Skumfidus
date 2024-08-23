# Installationsvejledning til Skumfidus Firmware

## Download af Firmware

1. Gå til [Skumfidus Releases-siden](https://github.com/OrangeMakers/Skumfidus/releases) på GitHub.
2. Find den seneste udgivelse (Er markeret med `latest`).
3. Download firmware-filen (f.eks. `firmware.bin`).

## Flashning af Firmware

Hvis du foretrækker at bruge kommandolinjen, kan du bruge esptool.py til at flashe firmwaren. Denne metode kræver, at Python er installeret på dit system.

1. Installer esptool.py, hvis du ikke allerede har gjort det:
   ```
   pip install esptool
   ```

2. Tilslut dit ESP32-board til din computer via USB.

3. Åbn en terminal eller kommandoprompt og naviger til mappen, der indeholder den downloadede firmware-fil.

4. Kør følgende kommando for at flashe firmwaren (erstat COM_PORT med din faktiske COM-port, og FIRMWARE_FILE.bin med det faktiske filnavn):
   ```
   esptool --chip esp32 --port COM_PORT --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x10000 FIRMWARE_FILE.bin
   ```

5. Vent på at processen er færdig. Du skulle gerne se en succesmeddelelse.

6. Nulstil dit ESP32-board.

Denne metode giver en mere fleksibel og skriptbar tilgang til at flashe din Skumfidus-enhed.
