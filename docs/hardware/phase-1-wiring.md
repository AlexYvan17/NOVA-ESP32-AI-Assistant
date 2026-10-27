# NOVA — Phase 1 Hardware Wiring

**Project:** NOVA ESP32-S3 AI Assistant  
**Phase:** 1 — Embedded Systems Fundamentals  
**Status:** Tested successfully  
**Date:** October 26, 2026

## Objective

The purpose of this circuit was to combine digital inputs and PWM output into a simple embedded control system.

The ESP32-S3 reads a KY-040 rotary encoder and a tactile push button to control the brightness of an external LED.

## Components Used

- ESP32-S3-WROOM-1-N16R8 development board
- KY-040 rotary encoder
- Red LED
- 330 Ω current-limiting resistor
- Tactile push button
- Breadboard
- Jumper wires
- USB-C cable

## GPIO Connections

| ESP32-S3 Pin | Connected Component | Function |
|---|---|---|
| GPIO4 | Red LED through 330 Ω resistor | PWM output |
| GPIO5 | Tactile push button | Reset brightness |
| GPIO6 | KY-040 CLK | Encoder clock |
| GPIO7 | KY-040 DT | Encoder direction |
| GPIO8 | KY-040 SW | Encoder push switch |
| 3V3 | KY-040 + | Encoder power |
| GND | LED cathode, button, KY-040 GND | Common ground |

## LED Circuit

The LED is connected as follows:

GPIO4 → 330 Ω resistor → LED anode (+) → LED cathode (−) → GND.

The resistor limits the current flowing through the LED.

The ESP32-S3 controls the LED's apparent brightness using pulse-width modulation (PWM).

## Tactile Push Button

The separate tactile button connects GPIO5 to GND when pressed.

The firmware configures GPIO5 using `INPUT_PULLUP`.

This means:

- Button released: HIGH
- Button pressed: LOW

Pressing the button resets LED brightness to 50% and enables the LED.

## Rotary Encoder

The KY-040 rotary encoder uses three signal connections:

- CLK → GPIO6
- DT → GPIO7
- SW → GPIO8

The encoder is powered using the ESP32-S3's 3.3 V supply.

Clockwise rotation increases brightness, while counterclockwise rotation decreases it.

Pressing the encoder's built-in switch toggles the LED ON or OFF.

## Safety Considerations

The KY-040 is powered from 3.3 V to maintain compatibility with the ESP32-S3's GPIO voltage levels.

The LED uses a current-limiting resistor.

USB power should be disconnected before modifying the breadboard connections.

## Testing

The circuit was tested using the PlatformIO Serial Monitor.

Successful tests included:

- Increasing brightness from 50% to 100%
- Decreasing brightness from 100% to 0%
- Maintaining brightness within the programmed limits
- Toggling the LED using the encoder push switch
- Resetting brightness to 50% using the separate tactile button

All five functional tests passed.

## Future Improvements

I plan to add a proper circuit schematic, photographs of the breadboard, and additional testing as NOVA develops.

The next phase will focus on connecting and programming the touchscreen.
