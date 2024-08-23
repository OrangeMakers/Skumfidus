## Description of next release

This release introduces WS2812B LED strip functionality to the project, enhancing visual feedback and user experience. The LED strip is initialized to display a solid orange color at full brightness when the system starts up. This addition provides a clear visual indicator that the system is powered on and ready for operation.

### Added
- Added function to initialize and turn on WS2812B LED strip
- Added LED strip initialization to setup function
- Added constant definitions for WS2812B LED strip (NUM_LEDS, LED_TYPE, COLOR_ORDER)
- Added FastLED library integration for controlling the LED strip

### Changed
- Updated CONVENTIONS.md with guidance for working with WS2812B LED strip
- Modified setup() function to initialize the LED strip

### Deprecated
- No changes

### Removed
- No changes

### Fixed
- No changes

### Security
- No changes
