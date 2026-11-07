
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Display configuration
const uint16_t BG_COLOR = TFT_BLACK;
const uint16_t EYE_COLOR = TFT_CYAN;

// Eye dimensions
const int LEFT_X = 46;
const int RIGHT_X = 148;
const int EYE_Y = 110;
const int EYE_W = 46;
const int EYE_H = 72;

// Available facial expressions
enum FaceState {
    NEUTRAL,
    HAPPY,
    SLEEPY,
    SURPRISED
};

FaceState currentFace = NEUTRAL;

// Draw the NOVA label
void drawLabel() {
    tft.setTextColor(EYE_COLOR, BG_COLOR);
    tft.setTextSize(2);
    tft.setCursor(96, 260);
    tft.print("NOVA");
}

// Draw neutral eyes
void drawNeutral() {
    tft.fillRoundRect(
        LEFT_X, EYE_Y, EYE_W, EYE_H,
        20, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, EYE_Y, EYE_W, EYE_H,
        20, EYE_COLOR
    );
}

// Draw happy eyes as upward curves
void drawHappy() {
    for (int x = 0; x <= EYE_W; x++) {
        float normalized = (x - EYE_W / 2.0f)
                         / (EYE_W / 2.0f);

        int y = 155 - (int)(
            28 * (1.0f - normalized * normalized)
        );

        tft.fillCircle(LEFT_X + x, y, 4, EYE_COLOR);
        tft.fillCircle(RIGHT_X + x, y, 4, EYE_COLOR);
    }
}

// Draw sleepy eyes
void drawSleepy() {
    tft.fillRoundRect(
        LEFT_X, 145, EYE_W, 6,
        3, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, 145, EYE_W, 6,
        3, EYE_COLOR
    );
}

// Draw surprised eyes
void drawSurprised() {
    tft.fillEllipse(
        LEFT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        23, 36,
        EYE_COLOR
    );

    tft.fillEllipse(
        RIGHT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        23, 36,
        EYE_COLOR
    );

    // Create hollow centers
    tft.fillEllipse(
        LEFT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        15, 28,
        BG_COLOR
    );

    tft.fillEllipse(
        RIGHT_X + EYE_W / 2,
        EYE_Y + EYE_H / 2,
        15, 28,
        BG_COLOR
    );
}

// Draw selected facial expression
void drawFace(FaceState state) {
    currentFace = state;

    // Clear previous face
    tft.fillScreen(BG_COLOR);

    switch (state) {
        case NEUTRAL:
            drawNeutral();
            Serial.println("Expression: NEUTRAL");
            break;

        case HAPPY:
            drawHappy();
            Serial.println("Expression: HAPPY");
            break;

        case SLEEPY:
            drawSleepy();
            Serial.println("Expression: SLEEPY");
            break;

        case SURPRISED:
            drawSurprised();
            Serial.println("Expression: SURPRISED");
            break;
    }

    drawLabel();
}

// Animate one blink
void blinkEyes() {
    for (int h = EYE_H; h >= 4; h -= 4) {
        tft.fillRect(LEFT_X, EYE_Y, EYE_W, EYE_H,
                     BG_COLOR);
        tft.fillRect(RIGHT_X, EYE_Y, EYE_W, EYE_H,
                     BG_COLOR);

        int y = EYE_Y + (EYE_H - h) / 2;
        int radius = min(20, h / 2);

        tft.fillRoundRect(
            LEFT_X, y, EYE_W, h, radius, EYE_COLOR
        );

        tft.fillRoundRect(
            RIGHT_X, y, EYE_W, h, radius, EYE_COLOR
        );

        delay(15);
    }

    delay(80);

    // Restore the neutral expression
    drawFace(NEUTRAL);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("NOVA Facial Expressions Starting");

    // Working SPI initialization sequence
    SPI.begin(12, -1, 11, 13);

    tft.init();
    tft.setRotation(0);

    tft.fillScreen(BG_COLOR);
    drawFace(NEUTRAL);

    Serial.println("NOVA facial system initialized");
}

void loop() {
    // Neutral expression
    drawFace(NEUTRAL);
    delay(1500);
    blinkEyes();
    delay(500);

    // Happy expression
    drawFace(HAPPY);
    delay(2500);

    // Sleepy expression
    drawFace(SLEEPY);
    delay(2500);

    // Surprised expression
    drawFace(SURPRISED);
    delay(2500);
}
