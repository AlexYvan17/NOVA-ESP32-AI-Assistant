# NOVA — ESP32-S3 AI Desktop Assistant

**An independent embedded systems and AI engineering project**

**Developer:** Doffou Alex Yvan Diaho  
**Status:** Phase 1 complete; Phase 2 next  
**Microcontroller:** ESP32-S3-WROOM-1-N16R8  
**Development timeline:** Approximately six months (flexible estimate)  
**Last updated:** October 26, 2026

## About NOVA

NOVA is a compact, Wi-Fi-connected desktop assistant that I am developing to explore embedded systems, electronics, C++, human–machine interfaces, and AI integration.

My goal is to build a device with an animated face, touchscreen controls, microphone, and speaker. I plan to connect it to cloud services for speech recognition, AI responses, and speech synthesis, and to add a small set of useful actions such as timers. Those features are **planned**; the current working prototype is an ESP32-S3 input/output controller.

I'm building NOVA in stages so I can understand and test each part before integrating it into the full system.

## Current progress

**Phase 1 — ESP32-S3 fundamentals: completed.** I have compiled and uploaded firmware, verified the board's memory, blinked an LED, read a tactile button, tested a KY-040 rotary encoder, and combined those components into a PWM LED brightness controller.

The integrated controller currently supports:

- Clockwise encoder rotation to increase brightness in 10% steps.
- Counterclockwise rotation to decrease brightness in 10% steps.
- Encoder-button press to toggle the LED on and off.
- Separate button press to restore 50% brightness and turn the LED on.
- Brightness limits of 0% to 100%.

The behavior was checked through Serial Monitor output and hands-on testing. See [the engineering log](docs/engineering-log.md) for the development story and test observations.

### Phase 1 — Working Prototype

![NOVA Phase 1 ESP32-S3 prototype](images/phase-1/phase%201%20prototype.jpeg)

*My first integrated NOVA prototype, featuring an ESP32-S3, KY-040 rotary encoder, tactile push button, and PWM-controlled LED.*

## Hardware

| Component | Role | Status |
|---|---|---|
| ESP32-S3-WROOM-1-N16R8 development board | Main controller | In use |
| Red LED and 330 Ω resistor | Output and PWM practice | Tested |
| Tactile push button | Digital input | Tested |
| KY-040 rotary encoder | Rotation and push input | Tested |
| Breadboard and jumper wires | Prototype wiring | In use |
| Waveshare 2-inch SPI capacitive touchscreen | Animated face and touch interface | Planned for Phase 2; integration not yet verified |
| INMP441 I²S microphone | Voice input | Planned |
| MAX98357A I²S amplifier and speaker | Voice output | Planned |
| IR components | Possible supported device control | Future work |

The ESP32-S3 reported **16 MB flash**, **8 MB PSRAM**, and a **240 MHz CPU** during firmware testing.

## Phase 1 wiring

| ESP32-S3 pin | Connection |
|---|---|
| GPIO4 | 330 Ω resistor → red LED anode; LED cathode → GND |
| GPIO5 | Separate tactile push button → GND (`INPUT_PULLUP`) |
| GPIO6 | KY-040 CLK |
| GPIO7 | KY-040 DT |
| GPIO8 | KY-040 SW |
| 3V3 | KY-040 + |
| GND | KY-040 GND and common ground |

**Electrical note:** The encoder module is powered from **3.3 V**, not 5 V. Power should be disconnected before changing wiring. This table documents the tested connections; a formal schematic and photographs are still to be added.

## Development environment

- Visual Studio Code and PlatformIO
- Arduino framework for ESP32
- C++
- USB serial connection and Serial Monitor at 115200 baud

The project uses the PlatformIO board target `esp32-s3-devkitc-1`, with configuration overrides for the 16 MB flash and 8 MB OPI PSRAM on my specific board. Local COM-port names can vary between computers.

## Planned architecture

1. **Local interface:** ESP32-S3 reads buttons, rotary encoder, microphone, and touchscreen.
2. **Connectivity:** Wi-Fi connects the device to a backend or authorized cloud services.
3. **AI pipeline:** Speech-to-text, language-model processing, and text-to-speech provide voice interaction.
4. **Action handling:** Structured commands are checked before NOVA performs supported tasks.
5. **Feedback:** The screen, speaker, and other outputs communicate status and responses.

These are design goals, not implemented features yet. Sensitive credentials should be kept out of the public firmware repository.

## Development roadmap

| Phase | Focus | Status |
|---|---|---|
| 1 | ESP32-S3 setup, GPIO, buttons, encoder, PWM | **Completed** |
| 2 | Touchscreen bring-up, graphics, animated face | **Next** |
| 3 | Wi-Fi connectivity and cloud integration | Planned |
| 4 | Microphone, speaker, and voice interaction | Planned |
| 5 | Useful tasks and validated commands | Planned |
| 6 | Enclosure and complete-system integration | Planned |
| 7 | Optional V2 features, including supported phone integration | Future |

## NOVA V1 success criteria

NOVA V1 will be considered complete when a standalone desktop prototype can:

- Display an animated face.
- Respond to touchscreen input.
- Accept a spoken question and play a spoken response.
- Carry out at least one validated task, such as a timer.

## Repository guide

- `src/main.cpp` — firmware source code.
- `platformio.ini` — PlatformIO build settings.
- `docs/engineering-log.md` — experiments, observations, and lessons learned.
- `docs/hardware/` — planned wiring diagrams and component documentation.
- `docs/testing/` — planned test records and evidence.

## Why I'm building this

I'm studying electrical engineering and want practical experience beyond coursework. NOVA gives me a way to practice firmware development, circuit prototyping, debugging, and eventually hardware/software integration. I'm documenting the work as I learn, including the problems I encounter and how I solve them.

**Next:** Bring up the touchscreen and begin the first animated face prototype.
