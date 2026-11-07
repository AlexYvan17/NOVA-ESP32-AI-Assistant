# NOVA — ESP32-S3 AI Desktop Assistant

NOVA is a personal embedded-systems engineering project focused on developing an interactive desktop AI assistant using the **ESP32-S3-WROOM-1-N16R8** microcontroller.

The goal is to combine embedded programming, electronics, graphical interfaces, audio processing, Wi-Fi connectivity, and cloud-based artificial intelligence into a functional desktop device.

## Project Status

**Phase 1: Hardware Bring-Up — Completed**

**Phase 2: Display and Facial Animation — In Progress**

### Completed Milestones

- ESP32-S3 development environment configured using VS Code and PlatformIO.
- GPIO output and LED control verified.
- PWM LED brightness control implemented.
- Push-button input tested.
- KY-040 rotary encoder integrated for brightness adjustment and LED control.
- 16 MB flash and 8 MB PSRAM verified.
- 2-inch ST7789 240×320 SPI display connected and initialized.
- SPI communication and color rendering verified.
- Text rendering implemented.
- Automatic eye blinking developed.
- Horizontal eye movement implemented.
- Four facial expressions developed: Neutral, Happy, Sleepy, and Surprised.

## Hardware

| Component | Purpose |
|---|---|
| ESP32-S3-WROOM-1-N16R8 | Main microcontroller |
| 2-inch ST7789 IPS TFT | Animated face and user interface |
| INMP441 I²S microphone | Voice input |
| MAX98357A I²S amplifier | Audio output |
| 8 Ω speaker | Speech and notification sounds |
| KY-040 rotary encoder | Physical input |
| WS2812B RGB LED | Status indication |
| IR transmitter and receiver | Future infrared control |
| Breadboard and jumper wires | Prototyping |

Some components listed above are planned for integration and are not yet part of the working firmware.

## Software and Development Tools

- Visual Studio Code
- PlatformIO
- Arduino framework for ESP32
- C++
- TFT_eSPI graphics library
- Git and GitHub

## Phase 1 — Hardware Bring-Up

Phase 1 focused on learning ESP32-S3 programming and verifying essential hardware functionality.

Implemented:

- LED blinking and GPIO control.
- PWM brightness adjustment.
- Push-button input using internal pull-up resistors.
- Rotary encoder position tracking.
- Serial Monitor debugging.
- Flash and PSRAM verification.

**Result:** The ESP32-S3 successfully performed the required hardware-control tasks.

## Phase 2 — Display and Facial Animation

Phase 2 focuses on building NOVA's graphical personality.

### Step 1 — ST7789 Display Integration

Configured the 240×320 ST7789 display using TFT_eSPI.

Verified:

- SPI initialization.
- Red, green, and blue screen rendering.
- Text rendering.
- Display orientation and graphical drawing functions.

### Step 2 — Animated Eyes

Developed a basic facial animation featuring:

- Two rounded cyan eyes.
- Automatic blinking.
- Horizontal eye movement.
- Improved rendering using partial screen updates.

### Step 3 — Facial Expressions

Implemented a reusable facial-expression system using a C++ enumeration and dedicated drawing functions.

Available expressions:

- **Neutral:** Standard open eyes.
- **Happy:** Curved smiling eyes.
- **Sleepy:** Narrow horizontal eyes.
- **Surprised:** Large outlined oval eyes.

The expressions can be selected programmatically, providing a foundation for future interaction-driven behavior.

### Current Display Pin Configuration

| ST7789 | ESP32-S3 |
|---|---|
| GND | GND |
| VCC | 3V3 |
| SCL | GPIO12 |
| SDA/MOSI | GPIO11 |
| RES | GPIO10 |
| DC | GPIO9 |
| CS | GPIO13 |
| BLK | 3V3 |

### Technical Notes

During initial testing, the ESP32-S3 did not complete TFT_eSPI initialization when `tft.init()` was called directly.

Explicitly initializing SPI before the display resolved the observed issue:

```cpp
SPI.begin(12, -1, 11, 13);
tft.init();
```

This initialization sequence is retained in the working firmware. The underlying cause has not yet been conclusively established.

## Development Roadmap

- [x] Phase 1 — ESP32-S3 hardware bring-up.
- [x] Phase 2 — ST7789 display initialization.
- [x] Phase 2 — Automatic blinking.
- [x] Phase 2 — Horizontal eye movement.
- [x] Phase 2 — Four facial expressions.
- [ ] Phase 2 — Nonblocking animation engine.
- [ ] Phase 2 — Touch interface integration, subject to compatible hardware.
- [ ] Phase 3 — Microphone and speaker integration.
- [ ] Phase 3 — Speech input and output.
- [ ] Phase 4 — Wi-Fi and cloud AI integration.
- [ ] Phase 5 — Interactive graphical menu and timers.
- [ ] Phase 6 — Additional connectivity and device controls.
- [ ] Phase 7 — Final enclosure and power integration.

## Long-Term Goal

NOVA is intended to become a functional desktop assistant capable of:

- Answering spoken questions.
- Displaying animated expressions.
- Setting timers and providing notifications.
- Interacting through a graphical interface.
- Connecting to cloud-based AI services.
- Supporting additional device-control capabilities.

The project is being developed incrementally, with each phase introducing new hardware, software, and system-integration challenges.

## Engineering Skills Developed

- Embedded C++ programming.
- ESP32-S3 firmware development.
- GPIO, PWM, and SPI communication.
- Hardware debugging and troubleshooting.
- TFT graphics programming.
- Animation and state-machine design.
- Modular embedded-system architecture.

## Project Status Summary

**Current milestone:** Phase 2 — Facial Expressions Completed.

**Next milestone:** Nonblocking facial-animation engine using `millis()`.

---

*An ongoing personal electrical engineering and embedded systems project.*
