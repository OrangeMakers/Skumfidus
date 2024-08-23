# Marsh Mellow 2.0 Projekt

## Projektbeskrivelse
Marsh Mellow 2.0 er et automatiseret system designet til at styre en steppermotor og et relæ med en ESP32 mikrocontroller. Systemet inkluderer også en LCD-skærm til at vise real-time information.

## Hovedfunktioner
1. Styring af steppermotor med præcis bevægelse
2. Periodisk aktivering af et relæ
3. Real-time visning af afstand og relæstatus på LCD-skærm
4. Brug af FreeRTOS til multitasking

## Hardware
- ESP32 DevKit V1
- 42HB34F08AB Steppermotor
- 16x2 I2C LCD-skærm
- Relæ modul
- Steppermotor driver (indstillet til 8 mikrostepping)

### Steppermotor Specifikationer
- Model: 42HB34F08AB
- Type: Hybrid Steppermotor
- Producent: Changzhou Bo Hong Electric Appliance Co.,Ltd
- Nominel spænding: 4.96V
- Nominel strøm: 0.8A
- Fase modstand: 6.2Ω ±10%
- Fase induktans: 10mH ±20%
- Step vinkel: 1.8° ±5% (fuld step, uden belastning)
- Holde moment: 2.4 kg·cm
- Antal ledninger: 4
- Rotor inerti: 34 g·cm²
- Motor vægt: 200g
- Motor længde: 34mm

## Software
Projektet er udviklet ved hjælp af PlatformIO og Arduino framework. Det bruger følgende biblioteker:
- AccelStepper til motorstyring
- LiquidCrystal_I2C til LCD-styring
- FreeRTOS til multitasking

## Opsætning
1. Tilslut hardwaren som beskrevet i [Hardware Setup](docs/Hardware_Setup.md)
2. Installer PlatformIO i din foretrukne IDE
3. Åbn projektet og upload koden til ESP32

For detaljerede instruktioner om installation af firmware, se venligst [INSTALL.md](INSTALL.md).

## Brug
Efter upload vil systemet:
1. Vise en velkomstbesked på LCD-skærmen
2. Begynde at bevæge steppermotoren frem og tilbage
3. Aktivere og deaktivere relæet periodisk
4. Vise den aktuelle afstand og relæstatus på LCD-skærmen

## Bidrag
Bidrag til projektet er velkomne. Åbn venligst et issue for at diskutere større ændringer før indsendelse af pull requests.

## Licens
Dette projekt er open source og tilgængeligt under MIT-licensen.

MIT License

Copyright (c) 2023 OrangeMakers

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
