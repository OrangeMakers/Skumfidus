# Installationsvejledning til Skumfidus Firmware

## Download af Firmware

1. Gå til [Skumfidus Releases-siden](https://github.com/OrangeMakers/Skumfidus/releases) på GitHub.
2. Find den seneste udgivelse (f.eks. v0.3.0-87).
3. Download firmware-filen (f.eks. `firmware-v0.3.0-87.bin`).

## Flashning af Firmware

For at flashe firmwaren skal du bruge ESP32 Flash Download Tool. Hvis du ikke har det, kan du downloade det fra [Espressif-hjemmesiden](https://www.espressif.com/en/support/download/other-tools).

1. Tilslut dit ESP32-board til din computer via USB.
2. Åbn ESP32 Flash Download Tool.
3. I værktøjet:
   - Vælg den COM-port, som din ESP32 er tilsluttet.
   - Indstil baud-raten til 115200.
   - Klik på "..." knappen ved siden af det første tomme felt og vælg den downloadede firmware-fil.
   - Indstil adressen til 0x10000.
   - Marker boksen ved siden af firmware-filen.
4. Klik på "Start" for at begynde flashningen.
5. Vent på at processen er færdig. Værktøjet vil vise "FINISH", når det er færdigt.
6. Nulstil dit ESP32-board.

Din Skumfidus-enhed skulle nu køre med den seneste firmware!

Bemærk: Sørg altid for at bruge den seneste firmware-version for at få den bedste ydeevne og funktioner.

## Alternativ Metode: Brug af esptool.py

Hvis du foretrækker at bruge kommandolinjen, kan du bruge esptool.py til at flashe firmwaren. Denne metode kræver, at Python er installeret på dit system.

1. Installer esptool.py, hvis du ikke allerede har gjort det:
   ```
   pip install esptool
   ```

2. Tilslut dit ESP32-board til din computer via USB.

3. Åbn en terminal eller kommandoprompt og naviger til mappen, der indeholder den downloadede firmware-fil.

4. Kør følgende kommando for at flashe firmwaren (erstat COM_PORT med din faktiske COM-port, og FIRMWARE_FILE.bin med det faktiske filnavn):
   ```
   esptool.py --chip esp32 --port COM_PORT --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x10000 FIRMWARE_FILE.bin
   ```

5. Vent på at processen er færdig. Du skulle gerne se en succesmeddelelse.

6. Nulstil dit ESP32-board.

Denne metode giver en mere fleksibel og skriptbar tilgang til at flashe din Skumfidus-enhed.
