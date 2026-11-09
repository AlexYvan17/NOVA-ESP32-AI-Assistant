
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// =====================================
// HARDWARE PINS
// =====================================

// KY-040 rotary encoder
const uint8_t CLK_PIN = 6;
const uint8_t DT_PIN = 7;
const uint8_t SW_PIN = 8;

// ST7789 display
// MOSI = GPIO11
// SCLK = GPIO12
// CS   = GPIO13
// DC   = GPIO9
// RST  = GPIO10

// =====================================
// DISPLAY SETTINGS
// =====================================

const uint16_t BG_COLOR = TFT_BLACK;
const uint16_t EYE_COLOR = TFT_CYAN;

const int LEFT_X = 46;
const int RIGHT_X = 148;
const int EYE_Y = 110;
const int EYE_W = 46;
const int EYE_H = 72;

// =====================================
// FACE STATES
// =====================================

enum FaceState {
    NEUTRAL,
    HAPPY,
    SLEEPY,
    SURPRISED
};

FaceState currentFace = NEUTRAL;

const char* faceNames[] = {
    "NEUTRAL",
    "HAPPY",
    "SLEEPY",
    "SURPRISED"
};

// =====================================
// BLINK STATE MACHINE
// =====================================

enum BlinkState {
    EYES_OPEN,
    EYES_CLOSING,
    EYES_OPENING
};

BlinkState blinkState = EYES_OPEN;

int blinkHeight = EYE_H;

unsigned long lastBlink = 0;
unsigned long lastBlinkFrame = 0;

const unsigned long BLINK_INTERVAL = 3000;
const unsigned long BLINK_FRAME_INTERVAL = 25;

const int BLINK_STEP = 6;

// =====================================
// ENCODER STATE MACHINE
// =====================================

// Transition lookup table
const int8_t transitionTable[16] = {
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
};

uint8_t previousEncoderState = 0;
int8_t encoderCount = 0;

// Change to -1 if directions are reversed
const int ENCODER_DIRECTION = 1;

// Button debounce
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 40;

// =====================================
// DISPLAY DRAWING FUNCTIONS
// =====================================

void drawLabel() {
    tft.setTextColor(EYE_COLOR, BG_COLOR);
    tft.setTextSize(2);
    tft.setCursor(96, 260);
    tft.print("NOVA");
}

void drawNeutral() {
    tft.fillRoundRect(
        LEFT_X, EYE_Y,
        EYE_W, EYE_H,
        20, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, EYE_Y,
        EYE_W, EYE_H,
        20, EYE_COLOR
    );
}

void drawHappy() {
    for (int x = 0; x <= EYE_W; x++) {
        float normalized =
            (x - EYE_W / 2.0f) /
            (EYE_W / 2.0f);

        int y = 155 -
            (int)(28 * (1.0f -
            normalized * normalized));

        tft.fillCircle(
            LEFT_X + x, y, 4, EYE_COLOR
        );

        tft.fillCircle(
            RIGHT_X + x, y, 4, EYE_COLOR
        );
    }
}

void drawSleepy() {
    tft.fillRoundRect(
        LEFT_X, 145,
        EYE_W, 6, 3, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, 145,
        EYE_W, 6, 3, EYE_COLOR
    );
}

void drawSurprised() {
    tft.fillEllipse(
        LEFT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        23, 36, EYE_COLOR
    );

    tft.fillEllipse(
        RIGHT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        23, 36, EYE_COLOR
    );

    tft.fillEllipse(
        LEFT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        15, 28, BG_COLOR
    );

    tft.fillEllipse(
        RIGHT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        15, 28, BG_COLOR
    );
}

// =====================================
// FACE CONTROL
// =====================================

void drawFace(FaceState state) {
    currentFace = state;

    // Clear only the eye area
    tft.fillRect(
        35, 95, 175, 105, BG_COLOR
    );

    switch (state) {
        case NEUTRAL:
            drawNeutral();
            break;

        case HAPPY:
            drawHappy();
            break;

        case SLEEPY:
            drawSleepy();
            break;

        case SURPRISED:
            drawSurprised();
            break;
    }

    Serial.print("Expression: ");
    Serial.println(faceNames[currentFace]);
}

void selectFace(FaceState newFace) {
    // Cancel any unfinished blink
    blinkState = EYES_OPEN;
    blinkHeight = EYE_H;
    lastBlink = millis();

    drawFace(newFace);
}

void changeFace(int direction) {
    int next =
        ((int)currentFace + direction + 4) % 4;

    selectFace((FaceState)next);
}

// =====================================
// NONBLOCKING BLINK ANIMATION
// =====================================

void drawBlinkFrame(int height) {
    tft.fillRect(
        LEFT_X - 2, EYE_Y - 2,
        EYE_W + 4, EYE_H + 4,
        BG_COLOR
    );

    tft.fillRect(
        RIGHT_X - 2, EYE_Y - 2,
        EYE_W + 4, EYE_H + 4,
        BG_COLOR
    );

    int y =
        EYE_Y + (EYE_H - height) / 2;

    int radius = min(20, height / 2);

    tft.fillRoundRect(
        LEFT_X, y,
        EYE_W, height,
        radius, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, y,
        EYE_W, height,
        radius, EYE_COLOR
    );
}

void updateBlink(unsigned long now) {
    if (currentFace != NEUTRAL) {
        return;
    }

    if (blinkState == EYES_OPEN) {
        if (now - lastBlink >=
            BLINK_INTERVAL) {

            blinkState = EYES_CLOSING;
            blinkHeight = EYE_H;
            lastBlinkFrame = now;
        }

        return;
    }

    if (now - lastBlinkFrame <
        BLINK_FRAME_INTERVAL) {
        return;
    }

    lastBlinkFrame = now;

    if (blinkState == EYES_CLOSING) {
        blinkHeight -= BLINK_STEP;

        if (blinkHeight <= 4) {
            blinkHeight = 4;
            blinkState = EYES_OPENING;
        }

        drawBlinkFrame(blinkHeight);
    }
    else if (blinkState == EYES_OPENING) {
        blinkHeight += BLINK_STEP;

        if (blinkHeight >= EYE_H) {
            blinkHeight = EYE_H;
            blinkState = EYES_OPEN;
            lastBlink = now;

            drawFace(NEUTRAL);
        }
        else {
            drawBlinkFrame(blinkHeight);
        }
    }
}

// =====================================
// ROTARY ENCODER INPUT
// =====================================

void updateEncoder() {
    uint8_t currentState =
        (digitalRead(CLK_PIN) << 1) |
         digitalRead(DT_PIN);

    if (currentState == previousEncoderState) {
        return;
    }

    uint8_t index =
        (previousEncoderState << 2) |
         currentState;

    encoderCount += transitionTable[index];

    previousEncoderState = currentState;

    // One complete quadrature cycle
    if (encoderCount >= 4) {
        encoderCount = 0;

        Serial.println("Encoder: CLOCKWISE");

        changeFace(ENCODER_DIRECTION);
    }
    else if (encoderCount <= -4) {
        encoderCount = 0;

        Serial.println("Encoder: COUNTERCLOCKWISE");

        changeFace(-ENCODER_DIRECTION);
    }
}

// =====================================
// ENCODER BUTTON
// =====================================

void updateButton(unsigned long now) {
    bool reading = digitalRead(SW_PIN);

    if (reading != lastButtonReading) {
        lastDebounceTime = now;
    }

    if (now - lastDebounceTime >=
        DEBOUNCE_MS) {

        if (reading != stableButtonState) {
            stableButtonState = reading;

            if (stableButtonState == LOW) {
                Serial.println("Encoder button pressed");

                selectFace(NEUTRAL);
            }
        }
    }

    lastButtonReading = reading;
}

// =====================================
// INITIALIZATION
// =====================================

void setup() {
    Serial.begin(115200);

    // Encoder inputs
    pinMode(CLK_PIN, INPUT_PULLUP);
    pinMode(DT_PIN, INPUT_PULLUP);
    pinMode(SW_PIN, INPUT_PULLUP);

    previousEncoderState =
        (digitalRead(CLK_PIN) << 1) |
         digitalRead(DT_PIN);

    // Display initialization
    // Preserve this working sequence
    SPI.begin(12, -1, 11, 13);

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(BG_COLOR);

    drawLabel();
    selectFace(NEUTRAL);

    Serial.println(
        "NOVA Interactive Face System Ready"
    );
}

// =====================================
// MAIN LOOP
// =====================================

void loop() {
    unsigned long now = millis();

    updateEncoder();
    updateButton(now);
    updateBlink(now);

    // Future NOVA tasks:
    // updateAudio();
    // updateWiFi();
    // updateTimers();
}
