# Installationsvejledning til Skumfidus Firmware

## Forudsætninger

Før du begynder at flashe firmwaren, skal du installere Python og esptool på dit system.

### Installation af Python

#### Windows:
1. Besøg [Python's officielle hjemmeside](https://www.python.org/downloads/windows/).
2. Download den seneste version af Python til Windows.
3. Kør installationsprogrammet og sørg for at markere "Add Python to PATH" under installationen.
4. Følg installationsguiden for at fuldføre installationen.

#### Linux:
Python er ofte forudinstalleret på Linux. Hvis ikke, kan du installere det ved hjælp af din distributions pakkehåndtering:

For Ubuntu/Debian:
```
sudo apt update
sudo apt install python3 python3-pip
```

For Fedora:
```
sudo dnf install python3 python3-pip
```

### Installation af esptool

Efter Python er installeret, kan du installere esptool ved hjælp af pip:

#### Windows:
Åbn en kommandoprompt og kør:
```
pip install esptool
```

#### Linux:
Åbn en terminal og kør:
```
pip3 install esptool
```

## Download af Firmware

1. Gå til [Skumfidus Releases-siden](https://github.com/OrangeMakers/Skumfidus/releases) på GitHub.
2. Find den seneste udgivelse (markeret med `latest`).
3. Download zip-filen med firmwaren (f.eks. `firmware.zip`).
4. Udpak zip-filen til en mappe på din computer.

## Flashning af Firmware

1. Tilslut dit ESP32-board til din computer via USB.

2. Naviger til mappen, hvor du udpakkede firmware-filerne.

3. Kør det relevante installationsscript:

   - For Windows: Dobbeltklik på `Install_Firmware.cmd` eller kør det fra en kommandoprompt.
   - For PowerShell: Højreklik på `Install_Firmware.ps1` og vælg "Kør med PowerShell".
   - For Linux: Åbn en terminal i mappen og kør `./Install_Firmware.sh`.

4. Følg instruktionerne i scriptet. Du vil blive bedt om at angive COM-porten (Windows) eller seriel port (Linux), eller du kan lade scriptet automatisk detektere den korrekte port.

5. Vent på at flashning-processen er færdig. Du skulle gerne se en succesmeddelelse.

6. Nulstil dit ESP32-board.

Denne metode sikrer, at alle nødvendige firmware-komponenter bliver flashet korrekt til din Skumfidus-enhed.

### Debug Firmware

Hvis du oplever problemer med din Skumfidus-enhed, kan du bruge debug-firmwaren for at få mere detaljeret information:

1. Download `firmware-debug.zip` i stedet for den normale `firmware.zip`.
2. Udpak og flash denne firmware som beskrevet ovenfor.
3. Efter flashning, åbn en seriel monitor (f.eks. Arduino IDE's serielle monitor) med en baud rate på 115200.
4. Du vil nu se detaljerede debug-informationer på den serielle monitor.

Bemærk: Debug-firmwaren bør kun bruges til fejlfinding, da den kan påvirke enhedens ydeevne.

## Fejlfinding

Hvis du oplever problemer under installationen eller flashning-processen, kan du prøve følgende:

1. Sørg for, at du har de nødvendige rettigheder til at køre scripts og installere software på din computer.
2. Kontroller, at dit ESP32-board er korrekt tilsluttet og genkendt af dit system.
3. Prøv at bruge en anden USB-port eller USB-kabel.
4. Hvis du bruger Windows, kan du muligvis være nødt til at installere [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers).

Hvis problemerne fortsætter, kan du søge hjælp i [Skumfidus GitHub Issues](https://github.com/OrangeMakers/Skumfidus/issues) eller kontakte support.
