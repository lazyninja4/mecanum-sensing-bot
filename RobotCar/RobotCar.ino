struct ControlData {
  int16_t joyX;
  int16_t joyY;
  bool    kickActive;
  bool    eyesMode;
};
ControlData ctrl = {0, 0, false, false};

void setup() {
  Serial.begin(115200);

  // i2c for oled — set pins before Wire.begin() on pico (pins defined on Pins.h)
  Wire.setSDA(PIN_I2C_SDA);
  Wire.setSCL(PIN_I2C_SCL);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  motorsInit();
  stepperInit();
  ultrasonicInit();

  kickServo.attach(PIN_SERVO);
  kickServo.write(90);

  // spi0 for nrf — set pins before radio.begin() on pico
  SPI.setRX(PIN_NRF_MISO);
  SPI.setTX(PIN_NRF_MOSI);
  SPI.setSCK(PIN_NRF_SCK);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  eyesInit();
}

void loop() {

  // receive radio packet
  if (radio.available()) {
    radio.read(&ctrl, sizeof(ctrl));
    eyesMode = ctrl.eyesMode;
  }

  // ultrasonic read
  float distCm = ultrasonicRead();
  obstacleBlocked = (!eyesMode) && (distCm > 0 && distCm < 20.0f);
  bool movingTowardObstacle = (ctrl.joyY > 0);

  // drive motors
  if (obstacleBlocked && movingTowardObstacle) {
    motorsStop();
  } else {
    driveFromJoystick(ctrl.joyX, ctrl.joyY);
  }

  // kick servo — non-blocking 400ms pulse
  if (ctrl.kickActive && !kickActive) {
    kickActive = true;
    kickTime = millis();
    kickServo.write(30);
  }
  if (kickActive && (millis() - kickTime > 400)) {
    kickActive = false;
    kickServo.write(90);
  }

  // stepper head sweep
  if (!eyesMode) {
    stepperTick();
  }

  // display
  if (eyesMode) {
    eyesDraw();
  } else {
    display.clearDisplay();
    display.print("Dist: ");
    display.print(distCm, 1);
    display.println(" cm");
    if (obstacleBlocked) display.println("!! blocked !!");
    display.display();
  }

  delay(20);
}
