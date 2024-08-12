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
- 42hs3417 Steppermotor
- 16x2 I2C LCD-skærm
- Relæ modul
- Steppermotor driver (indstillet til 8 mikrostepping)

## Software
Projektet er udviklet ved hjælp af PlatformIO og Arduino framework. Det bruger følgende biblioteker:
- AccelStepper til motorstyring
- LiquidCrystal_I2C til LCD-styring
- FreeRTOS til multitasking

## Opsætning
1. Tilslut hardwaren som beskrevet i `PINS.md`
2. Installer PlatformIO i din foretrukne IDE
3. Åbn projektet og upload koden til ESP32

## Brug
Efter upload vil systemet:
1. Vise en velkomstbesked på LCD-skærmen
2. Begynde at bevæge steppermotoren frem og tilbage
3. Aktivere og deaktivere relæet periodisk
4. Vise den aktuelle afstand og relæstatus på LCD-skærmen

## Bidrag
Bidrag til projektet er velkomne. Åbn venligst et issue for at diskutere større ændringer før indsendelse af pull requests.

## Licens
Dette projekt er open source og tilgængeligt under [indsæt licens her].
