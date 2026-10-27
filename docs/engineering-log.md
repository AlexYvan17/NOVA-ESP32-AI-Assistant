# NOVA Engineering Log

This is where I record what I build, what I test, what goes wrong, and what I learn while developing NOVA. The [README](../README.md) explains the overall project; this log focuses on the hands-on work.

---

# Phase 1 — Getting Started with the ESP32-S3

**Status:** Completed  
**Recorded:** October 26, 2026  
**Focus:** Firmware setup, GPIO, digital inputs, rotary encoder, and PWM

## Why I started here

NOVA's long-term goal is to become a small desktop assistant with an animated face and voice interaction. Before working on those features, I wanted to understand the ESP32-S3 and learn how to make physical components respond to code.

I decided to start small, test one component at a time, and combine everything only after the individual experiments worked.

## 1. Getting the development environment working

I used Visual Studio Code, PlatformIO, the Arduino framework, and C++. My first firmware printed **“Hello NOVA!”** to the Serial Monitor, followed by a running message. Seeing that output confirmed that I could compile code, upload it, and communicate with the board.

I then checked the board's memory and CPU information. My ESP32-S3 reported **16 MB flash**, **8 MB PSRAM**, and **240 MHz CPU frequency**.

The PSRAM test did not work correctly at first: it initially showed 0 MB. After updating the PlatformIO settings for my board and enabling PSRAM support, the program detected the full 8 MB. That taught me not to assume a board's default software configuration matches its actual hardware.

## 2. My first LED circuit

I connected a red LED and a 330 Ω current-limiting resistor to **GPIO4**, with the LED's cathode connected to ground.

The goal was simple: turn the LED on for one second and off for one second. After uploading the program, I watched it blink at the expected rate. This was my first confirmation that the ESP32-S3 could control an external component.

## 3. Adding a tactile button

Next, I connected a separate tactile push button between **GPIO5** and ground. I used `INPUT_PULLUP`, so the input reads HIGH when the button is released and LOW when pressed.

I programmed the LED to stay on while I held the button and turn off when I released it. The physical circuit behaved as expected. This helped me understand digital inputs, pull-up resistors, and active-low button logic.

## 4. Testing the KY-040 rotary encoder

I connected the encoder's **CLK, DT, and SW** pins to **GPIO6, GPIO7, and GPIO8**. I powered the module from **3V3** and connected its ground to the ESP32-S3 ground.

The first test was a counter in the Serial Monitor. Turning the knob clockwise increased the count; turning it counterclockwise decreased it. Pressing the knob reset the count to zero.

One successful test showed the counter going **1 → 2 → 3**, then back to **2 → 1**, followed by **“Button pressed | Counter reset: 0.”** It was satisfying to see the program distinguish rotation direction and a button press using the same component.

## 5. Bringing everything together: LED brightness controller

For the final Phase 1 experiment, I combined the LED, rotary encoder, and separate push button into one system.

I used **pulse-width modulation (PWM)** to change the LED's apparent brightness. The encoder changes the brightness in **10% steps**, from **0% to 100%**. Pressing the encoder knob toggles the LED on and off, while the separate tactile button restores **50% brightness** and turns the LED on.

### Wiring used

| ESP32-S3 pin | Component |
|---|---|
| GPIO4 | 330 Ω resistor and red LED |
| GPIO5 | Separate tactile push button to GND |
| GPIO6 | KY-040 CLK |
| GPIO7 | KY-040 DT |
| GPIO8 | KY-040 SW |
| 3V3 | KY-040 + |
| GND | LED, button, and KY-040 common ground |

### Test results

| Test | Observed Serial Monitor result | Outcome |
|---|---|---|
| Clockwise rotation | Brightness increased in 10% steps up to 100% | Passed |
| Counterclockwise rotation | Brightness decreased in 10% steps down to 0% | Passed |
| Upper and lower limits | Additional rotation kept reporting 100% or 0% | Passed |
| Encoder push switch | `Encoder button | LED: OFF` and `... ON` | Passed |
| Separate tactile button | `Reset button | Brightness: 50%` | Passed |

The reset message appeared three times in the final test output. That is consistent with multiple button presses, but if it happened from only one press, the button debounce behavior needs further investigation. I have not yet measured PWM duty cycle with an oscilloscope or performed a long-duration reliability test.

## What I learned

This phase taught me more than how to make an LED blink. I practiced writing and uploading C++, reading digital inputs, using internal pull-up resistors, processing encoder signals, generating PWM, and checking results in the Serial Monitor.

I also learned that debugging is part of the process. The PSRAM configuration issue was a useful reminder to verify the hardware and software settings rather than guessing. Testing each component separately made the final integration much easier.

## Improvements I want to make later

- Add a proper circuit schematic and clear photographs of the breadboard.
- Save representative Serial Monitor screenshots with the project.
- Improve encoder and button handling if contact bounce causes unwanted events.
- Keep the firmware organized as NOVA grows beyond one `main.cpp` file.

## What's next?

**Phase 2 is the touchscreen and animated face.** I want to bring up the display first, confirm that the screen and touch controller work, and then begin drawing and animating NOVA's eyes.

I'm glad I started with the fundamentals. Phase 1 gave me a working prototype and a much better understanding of how firmware and physical electronics interact.

**Phase 1 complete — on to Phase 2.**
