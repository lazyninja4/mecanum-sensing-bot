// Low-level: control one motor on an L298N
// speed: -255 (full reverse) to +255 (full forward), 0 = stop
static inline void setMotor(int in1, int in2, int en, int speed) {
  int s = constrain(abs(speed), 0, 255);
  analogWrite(en, s);       // PWM: controls speed
  if (speed > 0) {
    digitalWrite(in1, HIGH); // forward
    digitalWrite(in2, LOW);
  } else if (speed < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH); // reverse
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  // coast (not brake)
  }
}

// High-level: translate joystick → all 4 wheel speeds
void driveFromJoystick(int16_t joyX, int16_t joyY) {
  // Map ±512 joystick range → ±200 motor speed range
  int fwd    = map(joyY, -512, 512, -200, 200);
  int strafe = map(joyX, -512, 512, -200, 200);

  // Dead-zone: ignore tiny values (joystick drift)
  if (abs(fwd)    < 15) fwd    = 0;
  if (abs(strafe) < 15) strafe = 0;

  // Mecanum kinematics
  int fl = constrain(fwd + strafe, -255, 255);
  int fr = constrain(fwd - strafe, -255, 255);
  int rl = constrain(fwd - strafe, -255, 255);
  int rr = constrain(fwd + strafe, -255, 255);

  setAllWheels(fl, fr, rl, rr);
}