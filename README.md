# NOVA — ESP32-S3 AI Desktop Assistant

### An Embedded Systems and Artificial Intelligence Engineering Project

**Project Status:** In Development — Phase 1  
**Developer:** AlexYvan17  
**Platform:** ESP32-S3 N16R8  
**Development Timeline:** Approximately 6 months (flexible target)

---

## Project Overview

NOVA is a compact, Wi-Fi-connected AI desktop assistant designed and developed using the ESP32-S3 microcontroller.

The goal is to combine embedded systems, electronics, artificial intelligence, and hardware-software integration into a functional desktop device capable of understanding voice commands, responding through a speaker, displaying animated facial expressions, and executing useful tasks.

NOVA is being developed incrementally, with each phase focusing on practical engineering skills and measurable results.

## Project Objectives

- Develop reliable firmware for the ESP32-S3 using C++.
- Design an interactive touchscreen interface with animated facial expressions.
- Implement Wi-Fi connectivity and cloud-based AI services.
- Integrate a microphone, amplifier, and speaker for voice interaction.
- Execute validated commands such as timers and supported device controls.
- Design and assemble a compact desktop enclosure.
- Document hardware designs, software development, testing, and engineering decisions.

## Hardware Platform

| Component | Purpose | Status |
|---|---|---|
| ESP32-S3 N16R8 | Main microcontroller | Owned |
| KY-040 Rotary Encoder | Menu navigation and volume control | Owned |
| Tactile Push Buttons | User input | Owned |
| 2N2222 Transistors | Switching and driver circuits | Owned |
| 2-inch Capacitive Touchscreen | Display and graphical interface | Planned |
| INMP441 Microphone | Voice input | Planned |
| MAX98357A Amplifier | Audio output | Planned |
| Speaker | Voice playback | Planned |
| IR LED | Infrared device control | Planned |

## System Architecture

NOVA uses a layered architecture:

**User Interaction → ESP32-S3 Hardware → Wi-Fi Connectivity → Cloud AI Services → Validated Task Execution**

The ESP32-S3 handles local hardware interactions, graphical interfaces, and communication.

Cloud services provide advanced speech recognition, AI processing, and speech synthesis.

Task requests are translated into structured commands, validated, and executed through authorized hardware functions or external services.

Sensitive actions require appropriate authorization and confirmation.

## Development Roadmap

| Phase | Description | Status |
|---|---|---|
| 1 | ESP32 Fundamentals | In Progress |
| 2 | Touchscreen and Graphical Interface | Planned |
| 3 | Wi-Fi and Cloud Integration | Planned |
| 4 | Audio and Voice Interaction | Planned |
| 5 | Task Execution | Planned |
| 6 | Enclosure and Final Integration | Planned |
| 7 | V2 Extensions and Phone Integration | Future |

### Phase 1 — ESP32 Fundamentals

**Objective:** Establish a reliable hardware and firmware development environment.

Planned milestones:

1. Configure the ESP32-S3 development environment.
2. Upload the first firmware program: `Hello NOVA!`
3. Control LEDs using GPIO outputs.
4. Read push-button inputs.
5. Interface with the KY-040 rotary encoder.
6. Combine inputs and outputs into a functional prototype.
7. Document testing and debugging results.

**Expected Deliverable:** A working ESP32-S3 prototype demonstrating basic input/output control.

## Software and Technologies

Planned technologies include:

- C++ and embedded firmware development
- Visual Studio Code and PlatformIO
- Git and GitHub
- SPI, I²C, and I²S communication
- LVGL graphical user interfaces
- Wi-Fi and HTTPS communication
- Cloud AI APIs
- Structured task execution
- KiCad for electrical schematics

## Future Development — NOVA V2

Potential future extensions include:

- Bluetooth LE phone connectivity
- Supported phone automation integrations
- Wake-word detection
- Advanced smart-home integrations
- Custom printed circuit board

These features are not required for NOVA V1.

## Definition of Done — NOVA V1

NOVA V1 will be considered complete when the device can:

1. Operate as a standalone desktop prototype.
2. Display an animated face.
3. Respond to touchscreen interactions.
4. Receive a spoken question.
5. Generate and play a spoken response.
6. Execute at least one supported real-world task.

## Engineering Documentation

This repository will include:

- Firmware source code
- Hardware schematics and wiring diagrams
- Component inventory
- Engineering development logs
- Testing and validation results
- Prototype photographs
- Design decisions and troubleshooting notes

## Project Purpose

NOVA is an independent engineering project intended to strengthen practical skills in embedded systems, electronics, programming, communication protocols, system integration, and AI-assisted device development.

The project emphasizes understanding engineering principles, developing reliable solutions, and maintaining professional technical documentation.

---

**Current Focus:** Phase 1 — ESP32-S3 Fundamentals

*This repository will be updated as development progresses.*
