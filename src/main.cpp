
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Display colors
const uint16_t BG_COLOR = TFT_BLACK;
const uint16_t EYE_COLOR = TFT_CYAN;

// Eye dimensions
const int LEFT_X = 46;
const int RIGHT_X = 148;
const int EYE_Y = 110;
const int EYE_W = 46;
const int EYE_H = 72;

// Expression states
enum FaceState {
    NEUTRAL,
    HAPPY,
    SLEEPY,
    SURPRISED
};

FaceState currentFace = NEUTRAL;

// Animation states
enum BlinkState {
    EYES_OPEN,
    EYES_CLOSING,
    EYES_OPENING
};

BlinkState blinkState = EYES_OPEN;

// Timing
unsigned long lastExpressionChange = 0;
unsigned long lastBlink = 0;
unsigned long lastBlinkFrame = 0;

const unsigned long EXPRESSION_INTERVAL = 3000;
const unsigned long BLINK_INTERVAL = 1000;
const unsigned long BLINK_FRAME_INTERVAL = 25;

// Blink animation
int blinkHeight = EYE_H;
const int BLINK_STEP = 6;

// Draw the NOVA label
void drawLabel() {
    tft.setTextColor(EYE_COLOR, BG_COLOR);
    tft.setTextSize(2);
    tft.setCursor(96, 260);
    tft.print("NOVA");
}

// Neutral expression
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

// Happy expression
void drawHappy() {
    for (int x = 0; x <= EYE_W; x++) {
        float normalized =
            (x - EYE_W / 2.0f) / (EYE_W / 2.0f);

        int y = 155 -
            (int)(28 * (1.0f - normalized * normalized));

        tft.fillCircle(LEFT_X + x, y, 4, EYE_COLOR);
        tft.fillCircle(RIGHT_X + x, y, 4, EYE_COLOR);
    }
}

// Sleepy expression
void drawSleepy() {
    tft.fillRoundRect(
        LEFT_X, 145, EYE_W, 6, 3, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, 145, EYE_W, 6, 3, EYE_COLOR
    );
}

// Surprised expression
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

// Render selected expression
void drawFace(FaceState state) {
    currentFace = state;

    // Clear only the eye region
    tft.fillRect(35, 95, 175, 105, BG_COLOR);

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
}

// Draw one frame of the blink
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

    int y = EYE_Y + (EYE_H - height) / 2;
    int radius = min(20, height / 2);

    tft.fillRoundRect(
        LEFT_X, y, EYE_W, height,
        radius, EYE_COLOR
    );

    tft.fillRoundRect(
        RIGHT_X, y, EYE_W, height,
        radius, EYE_COLOR
    );
}

// Update blinking without delay()
void updateBlink(unsigned long now) {
    if (currentFace != NEUTRAL) {
        return;
    }

    if (blinkState == EYES_OPEN) {
        if (now - lastBlink >= BLINK_INTERVAL) {
            blinkState = EYES_CLOSING;
            blinkHeight = EYE_H;
            lastBlinkFrame = now;
        }
        return;
    }

    if (now - lastBlinkFrame < BLINK_FRAME_INTERVAL) {
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

// Change expression periodically
void updateExpression(unsigned long now) {
    if (now - lastExpressionChange <
        EXPRESSION_INTERVAL) {
        return;
    }

    lastExpressionChange = now;

    // Cancel any unfinished blink
    blinkState = EYES_OPEN;
    blinkHeight = EYE_H;
    lastBlink = now;

    FaceState nextFace =
        static_cast<FaceState>((currentFace + 1) % 4);

    drawFace(nextFace);
}

void setup() {
    Serial.begin(115200);

    // Explicit SPI initialization is required
    // for this working hardware configuration.
    SPI.begin(12, -1, 11, 13);

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(BG_COLOR);

    drawLabel();
    drawFace(NEUTRAL);

    unsigned long now = millis();
    lastExpressionChange = now;
    lastBlink = now;

    Serial.println("NOVA nonblocking animation ready");
}

void loop() {
    unsigned long now = millis();

    updateExpression(now);
    updateBlink(now);

    // Future tasks can run here:
    // readButtons();
    // updateTimers();
    // processAudio();
}
