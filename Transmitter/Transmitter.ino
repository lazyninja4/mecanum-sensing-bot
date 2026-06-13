#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TFT_eSPI.h>
#include "Colours.h"
#include "DrawUI.h"
#include "Wheels.h"

#define NRF_CE   4
#define NRF_CSN  5
RF24 radio(NRF_CE, NRF_CSN);
const byte address[6] = "00001";

#define JOY_X  34
#define JOY_Y  35
int JOY_X_CENTER = 2048;
int JOY_Y_CENTER = 2048;

#define BTN_KICK  32
#define BTN_MODE  33
bool lastKick = false;
bool lastMode = false;
bool eyesMode = false;

// ── Control packet (must match robotcar struct) ────

struct ControlData {
  int16_t joyX;
  int16_t joyY;
  bool    kickActive;
  bool    eyesMode;
};

TFT_eSPI tft = TFT_eSPI();

unsigned long lastSend = 0;
unsigned long lastDraw = 0;

void setup() {
  pinMode(BTN_KICK, INPUT);
  pinMode(BTN_MODE, INPUT);

  delay(100);
  JOY_X_CENTER = analogRead(JOY_X);
  JOY_Y_CENTER = analogRead(JOY_Y);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(GRV_BG);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  drawUIBase(&tft, eyesMode);
}

void loop() {
  unsigned long now = millis();

  int16_t jX = constrain(analogRead(JOY_X) - JOY_X_CENTER, -512, 512);
  int16_t jY = constrain(analogRead(JOY_Y) - JOY_Y_CENTER, -512, 512);
  jY = -jY;
  if (abs(jX) < 30) jX = 0;
  if (abs(jY) < 30) jY = 0;

  bool kickBtn = (digitalRead(BTN_KICK) == LOW);
  bool kickActive = false;
  if (kickBtn && !lastKick) {
    kickActive = true;
    addLog("Kicked");
    drawKickButton(&tft, true);
  } else if (!kickBtn && lastKick) {
    drawKickButton(&tft, false);
  }
  lastKick = kickBtn;

  bool modeBtn = (digitalRead(BTN_MODE) == LOW);
  if (modeBtn && !lastMode) {
    eyesMode = !eyesMode;
    addLog(eyesMode ? "Eyes" : "Sensor");
    drawModeButton(&tft, eyesMode);
  }
  lastMode = modeBtn;

  if (now - lastSend >= 30) {
    lastSend = now;
    ControlData pkt = { jX, jY, kickActive, eyesMode };
    radio.write(&pkt, sizeof(pkt));
  }

  if (now - lastDraw >= 50) {
    lastDraw = now;
    updateJoystickDot(&tft, jX, jY);
    updateWheelIndicators(&tft, jX, jY);
    drawLogPanel(&tft);
  }
}
