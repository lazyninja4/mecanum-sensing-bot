#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Colours.h"

// Portrait 128x160
#define SCR_W  128
#define SCR_H  160

// Title row
#define TITLE_H   24

// Button grid (2 columns x 2 rows, top-right area)
#define BTN_W     28
#define BTN_H     28
#define BTN_GAP    6
#define BTN_X0    66   // left edge of button grid
#define BTN_Y0    (TITLE_H + 6)

// K and + buttons (left side, below title)
#define KBTN_W    28
#define KBTN_H    28
#define KBTN_X     6
#define KBTN_K_Y  (TITLE_H + 6)
#define KBTN_P_Y  (KBTN_K_Y + KBTN_H + BTN_GAP)

// Mode button (big yellow pill, top-left)
#define MODE_X     6
#define MODE_Y    (TITLE_H + 6)
#define MODE_W    52
#define MODE_H    18

// Log panel (right side, middle)
#define LOG_X     66
#define LOG_Y     (BTN_Y0 + 2*(BTN_H + BTN_GAP) + 6)
#define LOG_W     (SCR_W - LOG_X - 4)
#define LOG_LINES  4
#define LOG_LINE_H 13

// Joystick circle (bottom-left)
#define JOY_CX    34
#define JOY_CY    130
#define JOY_R     26
#define DOT_R      4

static int _prevDotX = JOY_CX;
static int _prevDotY = JOY_CY;

// ── Log storage ──────────────────────────────────────────────
static String _logLines[LOG_LINES] = {"", "", "", ""};

void addLog(const String& msg) {
  for (int i = 0; i < LOG_LINES - 1; i++) _logLines[i] = _logLines[i + 1];
  _logLines[LOG_LINES - 1] = msg;
}

// ── Individual draw helpers ───────────────────────────────────
static void drawGridButton(TFT_eSPI* tft, int col, int row, bool active) {
  int x = BTN_X0 + col * (BTN_W + BTN_GAP);
  int y = BTN_Y0 + row * (BTN_H + BTN_GAP);
  tft->fillRoundRect(x, y, BTN_W, BTN_H, 4, active ? GRV_YELLOW_B : GRV_BG2);
}

void drawKickButton(TFT_eSPI* tft, bool active) {
  tft->fillRoundRect(KBTN_X, KBTN_K_Y, KBTN_W, KBTN_H, 4, active ? GRV_ORANGE_B : GRV_BG2);
  tft->setTextColor(active ? GRV_BG : GRV_FG, active ? GRV_ORANGE_B : GRV_BG2);
  tft->setTextDatum(MC_DATUM);
  tft->drawString("K", KBTN_X + KBTN_W/2, KBTN_K_Y + KBTN_H/2, 2);
}

void drawModeButton(TFT_eSPI* tft, bool eyesMode) {
  uint16_t bg = eyesMode ? GRV_YELLOW_B : GRV_BG2;
  tft->fillRoundRect(MODE_X, MODE_Y, MODE_W, MODE_H, 6, bg);
  // Two dots inside (like the photo)
  uint16_t dotCol = eyesMode ? GRV_BG : GRV_BG4;
  tft->fillCircle(MODE_X + 14, MODE_Y + MODE_H/2, 5, dotCol);
  tft->fillCircle(MODE_X + 32, MODE_Y + MODE_H/2, 5, dotCol);
}

void drawLogPanel(TFT_eSPI* tft) {
  tft->fillRect(LOG_X, LOG_Y, LOG_W, LOG_LINES * LOG_LINE_H + 4, GRV_BG);
  tft->setTextDatum(ML_DATUM);
  for (int i = 0; i < LOG_LINES; i++) {
    bool latest = (i == LOG_LINES - 1);
    tft->setTextColor(latest ? GRV_FG : GRV_BLUE_B, GRV_BG);
    String line = latest ? (">" + _logLines[i]) : (" " + _logLines[i]);
    tft->drawString(line, LOG_X, LOG_Y + i * LOG_LINE_H + 6, 1);
  }
}

void updateJoystickDot(TFT_eSPI* tft, int16_t jX, int16_t jY) {
  // Erase old dot
  tft->fillCircle(_prevDotX, _prevDotY, DOT_R + 1, GRV_BG1);
  // Restore crosshair
  tft->drawFastHLine(JOY_CX - JOY_R, JOY_CY, JOY_R * 2, GRV_BG3);
  tft->drawFastVLine(JOY_CX, JOY_CY - JOY_R, JOY_R * 2, GRV_BG3);
  tft->drawCircle(JOY_CX, JOY_CY, JOY_R, GRV_BG4);

  int nx = JOY_CX + map(jX, -512, 512, -JOY_R, JOY_R);
  int ny = JOY_CY - map(jY, -512, 512, -JOY_R, JOY_R);
  nx = constrain(nx, JOY_CX - JOY_R + DOT_R, JOY_CX + JOY_R - DOT_R);
  ny = constrain(ny, JOY_CY - JOY_R + DOT_R, JOY_CY + JOY_R - DOT_R);

  tft->fillCircle(nx, ny, DOT_R, GRV_YELLOW_B);
  _prevDotX = nx;
  _prevDotY = ny;
}

void updateWheelIndicators(TFT_eSPI* tft, int16_t jX, int16_t jY) {
  int fwd    = map(jY, -512, 512, -255, 255);
  int strafe = map(jX, -512, 512, -255, 255);
  if (abs(fwd)    < 10) fwd    = 0;
  if (abs(strafe) < 10) strafe = 0;

  int speeds[4] = {
    fwd + strafe,  // FL col=0 row=0
    fwd - strafe,  // FR col=1 row=0
    fwd - strafe,  // RL col=0 row=1
    fwd + strafe   // RR col=1 row=1
  };
  int cols[4] = {0, 1, 0, 1};
  int rows[4] = {0, 0, 1, 1};

  for (int i = 0; i < 4; i++) {
    bool active = abs(speeds[i]) > 10;
    bool forward = speeds[i] > 0;
    int x = BTN_X0 + cols[i] * (BTN_W + BTN_GAP);
    int y = BTN_Y0 + rows[i] * (BTN_H + BTN_GAP);
    uint16_t col = !active ? GRV_BG2 : (forward ? GRV_GREEN_B : GRV_RED_B);
    tft->fillRoundRect(x, y, BTN_W, BTN_H, 4, col);
  }
}

// ── Full initial draw ─────────────────────────────────────────
void drawUIBase(TFT_eSPI* tft, bool eyesMode) {
  tft->fillScreen(GRV_BG);

  // Title
  tft->setTextColor(GRV_YELLOW_B, GRV_BG);
  tft->setTextDatum(MC_DATUM);
  tft->drawString("ROBOTCAR", SCR_W / 2, TITLE_H / 2, 4);

  // Mode button
  drawModeButton(tft, eyesMode);

  // K button
  drawKickButton(tft, false);

  // + button (placeholder — extend ControlData if you want it to do something)
  tft->fillRoundRect(KBTN_X, KBTN_P_Y, KBTN_W, KBTN_H, 4, GRV_BG2);
  tft->setTextColor(GRV_FG, GRV_BG2);
  tft->setTextDatum(MC_DATUM);
  tft->drawString("+", KBTN_X + KBTN_W/2, KBTN_P_Y + KBTN_H/2, 2);

  // 2x2 wheel grid
  for (int r = 0; r < 2; r++)
    for (int c = 0; c < 2; c++)
      drawGridButton(tft, c, r, false);

  // Joystick circle
  tft->fillCircle(JOY_CX, JOY_CY, JOY_R, GRV_BG1);
  tft->drawFastHLine(JOY_CX - JOY_R, JOY_CY, JOY_R * 2, GRV_BG3);
  tft->drawFastVLine(JOY_CX, JOY_CY - JOY_R, JOY_R * 2, GRV_BG3);
  tft->drawCircle(JOY_CX, JOY_CY, JOY_R, GRV_BG4);
  tft->fillCircle(JOY_CX, JOY_CY, DOT_R, GRV_YELLOW_B);

  // Log panel
  drawLogPanel(tft);
}
