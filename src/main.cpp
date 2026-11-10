#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// NOVA Phase 2 Step 6C: interactive faces with two small 8-bit sprites.
// Keep your proven platformio.ini and hardware wiring unchanged.
TFT_eSPI tft;
TFT_eSprite leftEye(&tft);
TFT_eSprite rightEye(&tft);

constexpr uint8_t CLK_PIN = 6, DT_PIN = 7, SW_PIN = 8;
constexpr int LEFT_X = 46, RIGHT_X = 148, EYE_Y = 110;
constexpr int EYE_W = 46, EYE_H = 72;
constexpr int SPRITE_W = 50, SPRITE_H = 76;
constexpr int SPRITE_Y = EYE_Y - 2;
constexpr uint32_t FRAME_MS = 40;

enum FaceState { NEUTRAL, HAPPY, SLEEPY, SURPRISED };
FaceState currentFace = NEUTRAL;
const char *const faceNames[] = {"NEUTRAL", "HAPPY", "SLEEPY", "SURPRISED"};

// Quadrature table retained from the working Step 5C encoder firmware.
const int8_t transitionTable[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};
uint8_t previousEncoderState = 0;
int8_t encoderCount = 0;
constexpr int ENCODER_DIRECTION = 1;
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
uint32_t lastDebounceTime = 0;
constexpr uint32_t DEBOUNCE_MS = 40;

enum BlinkPhase { OPEN, CLOSING, OPENING };
BlinkPhase blinkPhase = OPEN;
int eyeHeight = EYE_H;
int gazeX = 0, targetGazeX = 0;
uint32_t nextBlinkAt = 0, nextLookAt = 0, lastFrameAt = 0;

void scheduleBlink(uint32_t now) {
  nextBlinkAt = now + (uint32_t)random(3000, 7001);
}
void scheduleLook(uint32_t now) {
  nextLookAt = now + (uint32_t)random(2000, 5001);
}

void drawOneEye(TFT_eSprite &sprite, int screenX) {
  sprite.fillSprite(TFT_BLACK);
  switch (currentFace) {
    case NEUTRAL: {
      const int y = 2 + (EYE_H - eyeHeight) / 2;
      const int radius = min(20, eyeHeight / 2);
      sprite.fillRoundRect(2 + gazeX, y, EYE_W, eyeHeight, radius, TFT_CYAN);
      break;
    }
    case HAPPY: {
      // Smiling curved eye, translated into sprite-local coordinates.
      for (int x = 0; x <= EYE_W; ++x) {
        const float n = (x - EYE_W / 2.0f) / (EYE_W / 2.0f);
        const int globalY = 155 - (int)(28 * (1.0f - n * n));
        sprite.fillCircle(2 + x, globalY - SPRITE_Y, 4, TFT_CYAN);
      }
      break;
    }
    case SLEEPY:
      sprite.fillRoundRect(2, 145 - SPRITE_Y, EYE_W, 6, 3, TFT_CYAN);
      break;
    case SURPRISED:
      sprite.fillEllipse(25, 38, 23, 36, TFT_CYAN);
      sprite.fillEllipse(25, 38, 15, 28, TFT_BLACK);
      break;
  }
  sprite.pushSprite(screenX, SPRITE_Y);
}

void renderEyes() {
  drawOneEye(leftEye, LEFT_X - 2);
  drawOneEye(rightEye, RIGHT_X - 2);
}

void selectFace(FaceState face) {
  currentFace = face;
  blinkPhase = OPEN;
  eyeHeight = EYE_H;
  gazeX = targetGazeX = 0;
  const uint32_t now = millis();
  scheduleBlink(now);
  scheduleLook(now);
  renderEyes();
  Serial.print("Expression: ");
  Serial.println(faceNames[(int)currentFace]);
}

void changeFace(int direction) {
  const int next = ((int)currentFace + direction + 4) % 4;
  selectFace((FaceState)next);
}

void updateEncoder() {
  const uint8_t state = (digitalRead(CLK_PIN) << 1) | digitalRead(DT_PIN);
  if (state == previousEncoderState) return;
  const uint8_t index = (previousEncoderState << 2) | state;
  encoderCount += transitionTable[index];
  previousEncoderState = state;
  if (encoderCount >= 4) {
    encoderCount = 0;
    Serial.println("Encoder: CLOCKWISE");
    changeFace(ENCODER_DIRECTION);
  } else if (encoderCount <= -4) {
    encoderCount = 0;
    Serial.println("Encoder: COUNTERCLOCKWISE");
    changeFace(-ENCODER_DIRECTION);
  }
}

void updateButton(uint32_t now) {
  const bool reading = digitalRead(SW_PIN);
  if (reading != lastButtonReading) lastDebounceTime = now;
  if (now - lastDebounceTime >= DEBOUNCE_MS && reading != stableButtonState) {
    stableButtonState = reading;
    if (stableButtonState == LOW) {
      Serial.println("Encoder button pressed");
      selectFace(NEUTRAL);
    }
  }
  lastButtonReading = reading;
}

void updateAnimation(uint32_t now) {
  if (currentFace != NEUTRAL || now - lastFrameAt < FRAME_MS) return;
  lastFrameAt = now;
  bool changed = false;

  if (blinkPhase == OPEN && (int32_t)(now - nextBlinkAt) >= 0) {
    blinkPhase = CLOSING;
  }
  if (blinkPhase == CLOSING) {
    eyeHeight -= 12;
    if (eyeHeight <= 4) {
      eyeHeight = 4;
      blinkPhase = OPENING;
    }
    changed = true;
  } else if (blinkPhase == OPENING) {
    eyeHeight += 12;
    if (eyeHeight >= EYE_H) {
      eyeHeight = EYE_H;
      blinkPhase = OPEN;
      scheduleBlink(now);
    }
    changed = true;
  }

  if ((int32_t)(now - nextLookAt) >= 0 && gazeX == targetGazeX) {
    const int positions[] = {-2, 0, 2};
    int next = gazeX;
    // Select a different position to ensure visible gaze changes.
    while (next == gazeX) next = positions[random(0, 3)];
    targetGazeX = next;
    scheduleLook(now);
  }
  if (gazeX != targetGazeX) {
    gazeX += (targetGazeX > gazeX) ? 1 : -1;
    changed = true;
  }

  if (changed) renderEyes();
}

void setup() {
  Serial.begin(115200);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  previousEncoderState = (digitalRead(CLK_PIN) << 1) | digitalRead(DT_PIN);

  // Critical: SPI.begin must precede tft.init on NOVA hardware.
  SPI.begin(12, -1, 11, 13);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(96, 260);
  tft.print("NOVA");

  leftEye.setColorDepth(8);
  rightEye.setColorDepth(8);
  if (leftEye.createSprite(SPRITE_W, SPRITE_H) == nullptr ||
      rightEye.createSprite(SPRITE_W, SPRITE_H) == nullptr) {
    Serial.println("ERROR: sprite allocation failed");
    while (true) delay(1000);
  }
  randomSeed(micros());
  selectFace(NEUTRAL);
  Serial.println("NOVA Step 6C: interactive dual-sprite faces ready");
}

void loop() {
  const uint32_t now = millis();
  updateEncoder();
  updateButton(now);
  updateAnimation(now);
}
