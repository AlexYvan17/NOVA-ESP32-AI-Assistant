# NOVA — Phase 1 Test Results

**Date:** October 26, 2026  
**Hardware:** ESP32-S3-WROOM-1-N16R8  
**Development environment:** VS Code, PlatformIO, Arduino framework  
**Status:** Functional tests completed successfully

## 1. Testing Objective

The goal of these tests was to verify that the ESP32-S3 could communicate with external components and correctly process different input signals.

The final experiment combined a rotary encoder, tactile push button, and LED into a PWM brightness controller.

## 2. Hardware Configuration

| GPIO | Function |
|---|---|
| GPIO4 | LED PWM output |
| GPIO5 | Separate reset button |
| GPIO6 | Rotary encoder CLK |
| GPIO7 | Rotary encoder DT |
| GPIO8 | Rotary encoder push switch |

The KY-040 encoder was powered using 3.3 V.

The LED was connected through a 330 Ω current-limiting resistor.

## 3. Test Results

| Test | Expected Behavior | Observed Result | Status |
|---|---|---|---|
| Firmware upload | Program uploads successfully | PlatformIO reported SUCCESS | PASS |
| Flash verification | Detect 16 MB flash | 16 MB detected | PASS |
| PSRAM verification | Detect 8 MB PSRAM | 8 MB detected | PASS |
| LED blinking | LED blinks every second | LED blinked correctly | PASS |
| Tactile button | LED responds to button presses | Correct ON/OFF behavior | PASS |
| Encoder clockwise | Counter increases | Counter increased | PASS |
| Encoder counterclockwise | Counter decreases | Counter decreased | PASS |
| Encoder push switch | Counter resets | Counter reset to zero | PASS |
| PWM brightness increase | Brightness increases in 10% steps | Successfully reached 100% | PASS |
| PWM brightness decrease | Brightness decreases in 10% steps | Successfully reached 0% | PASS |
| Brightness limits | Remain between 0% and 100% | Limits maintained | PASS |
| Encoder button toggle | Toggle LED ON/OFF | Successfully toggled | PASS |
| Separate reset button | Reset brightness to 50% | Successfully reset | PASS |

## 4. Representative Serial Monitor Output

The following output was recorded during testing:

```text
Clockwise | Brightness: 100%
Counterclockwise | Brightness: 90%
Counterclockwise | Brightness: 80%
Reset button | Brightness: 50%
```

Additional testing confirmed that the encoder button could toggle the LED:

```text
Encoder button | LED: OFF
Encoder button | LED: ON
```

## 5. Observations

The system responded correctly to the tested inputs.

The rotary encoder successfully adjusted brightness in both directions.

The firmware prevented brightness values from exceeding the programmed limits.

The separate push button successfully restored the default brightness.

The reset message appeared multiple times during testing. Further investigation would be needed to determine whether those messages corresponded to separate button presses or mechanical contact bounce.

## 6. Testing Limitations

The initial testing focused on functional behavior.

The following tests have not yet been performed:

- Oscilloscope measurement of PWM signals.
- Long-duration reliability testing.
- Formal rotary encoder debounce characterization.
- Electrical measurements of LED current.

## 7. Conclusion

Phase 1 testing confirmed that the ESP32-S3 could successfully control external hardware and process multiple user inputs.

The integrated LED brightness controller passed its initial functional tests.

The results establish a foundation for NOVA's next development phase: touchscreen integration and graphical interface development.
