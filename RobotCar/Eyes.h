#pragma once
#include <Wire.h>
#include "Face.h"

Face* face = nullptr;

void eyesInit() {
    face = new Face(128, 64, 40);
    face->Expression.GoTo_Normal();
    face->RandomBlink = true;
    face->Blink.Timer.SetIntervalMillis(4000);
    face->RandomLook = true;
    face->RandomBehavior = true;
}

void eyesDraw() {
    if (face == nullptr) return;  // safety check
    face->Update();
}