#include <Arduino.h>

// NOVA GPIO configuration
const int LED_PIN = 4;
const int RESET_BUTTON_PIN = 5;
const int CLK_PIN = 6;
const int DT_PIN = 7;
const int ENCODER_SW_PIN = 8;

// LED settings
int brightness = 50;
bool ledEnabled = true;

// Encoder tracking
int lastCLKState;

// Button tracking
bool lastEncoderButton = HIGH;
bool lastResetButton = HIGH;

unsigned long lastEncoderPress = 0;
unsigned long lastResetPress = 0;

const unsigned long DEBOUNCE_TIME = 200;

void updateLED() {
    int pwmValue = ledEnabled
        ? map(brightness, 0, 100, 0, 255)
        : 0;

    ledcWrite(LED_PIN, pwmValue);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(CLK_PIN, INPUT_PULLUP);
    pinMode(DT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

    // Configure PWM: GPIO4, 5 kHz, 8-bit resolution
    if (!ledcAttach(LED_PIN, 5000, 8)) {
        Serial.println("ERROR: PWM initialization failed!");
        return;
    }

    lastCLKState = digitalRead(CLK_PIN);

    updateLED();

    Serial.println("==============================");
    Serial.println(" NOVA INTEGRATED CONTROL");
    Serial.println("==============================");
    Serial.println("Initial brightness: 50%");
}

void loop() {
    // Rotary encoder
    int currentCLKState = digitalRead(CLK_PIN);

    if (lastCLKState == HIGH && currentCLKState == LOW) {

        if (digitalRead(DT_PIN) == HIGH) {
            brightness = constrain(brightness + 10, 0, 100);
            Serial.printf("Clockwise | Brightness: %d%%\n", brightness);
        } else {
            brightness = constrain(brightness - 10, 0, 100);
            Serial.printf("Counterclockwise | Brightness: %d%%\n", brightness);
        }

        updateLED();
    }

    lastCLKState = currentCLKState;

    // KY-040 push switch
    bool encoderButton = digitalRead(ENCODER_SW_PIN);

    if (encoderButton == LOW &&
        lastEncoderButton == HIGH &&
        millis() - lastEncoderPress > DEBOUNCE_TIME) {

        ledEnabled = !ledEnabled;
        updateLED();

        Serial.printf("Encoder button | LED: %s\n",
                      ledEnabled ? "ON" : "OFF");

        lastEncoderPress = millis();
    }

    lastEncoderButton = encoderButton;

    // Separate reset button
    bool resetButton = digitalRead(RESET_BUTTON_PIN);

    if (resetButton == LOW &&
        lastResetButton == HIGH &&
        millis() - lastResetPress > DEBOUNCE_TIME) {

        brightness = 50;
        ledEnabled = true;

        updateLED();

        Serial.println("Reset button | Brightness: 50%");

        lastResetPress = millis();
    }

    lastResetButton = resetButton;

    delay(1);
}
