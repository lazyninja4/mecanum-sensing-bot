#pragma once
#include <Arduino.h>
#include "Pins.h"

static const int STEP_SEQUENCE[8][4] = {
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
};

static int _stepIndex = 0;
static int _stepDir   = 1;
static int _stepPos   = 0;
static const int SWEEP_RANGE = 256;
static unsigned long _lastStepTime = 0;
static const unsigned long STEP_DELAY_US = 2000;
static int _stepPins[4] = {PIN_STEP_IN1, PIN_STEP_IN2, PIN_STEP_IN3, PIN_STEP_IN4};

void stepperInit() {
  for (int p : _stepPins) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
  }
}

// call every loop() — advances one step, non-blocking
void stepperTick() {
  if (micros() - _lastStepTime < STEP_DELAY_US) return;
  _lastStepTime = micros();

  for (int i = 0; i < 4; i++)
    digitalWrite(_stepPins[i], STEP_SEQUENCE[_stepIndex][i]);

  _stepIndex = (_stepIndex + _stepDir + 8) % 8;
  _stepPos  += _stepDir;

  if (_stepPos >=  SWEEP_RANGE) _stepDir = -1;
  if (_stepPos <= -SWEEP_RANGE) _stepDir =  1;
}

void stepperRelease() {
  for (int p : _stepPins) digitalWrite(p, LOW);
}
