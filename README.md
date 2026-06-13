# mecanum-sensing-bot
I had built this bot originally for the purpose of playing soccer - hence its body and sensing functions are intended to efficiently move and "kick" the ball, and it's also why I included a joystick system as a remote control instead of plain wi-fi. The mecanum wheels help in omni-directional movement. However, this project became my kickstart into the field of signal processing, communication and electronic circuits.
# Bot Description
The robot is made of two separate systems that communicate via NRF24L01 Radio modules operating at 2.4Ghz: the robot car, and the transmitter. The main microcontroller used in the robot as the brain is the Raspberry Pi Pico RP2040, and for the transmitter, it's the ESP32 Devkit V1.  
### Robot Car Components:-  
- Raspberry Pi Pico RP2040: The brain. Code was programmed in Arduino in C++, as I was more comfortable with it than MicroPython.
- BO Motors + Mecanum wheels (x4): Roller orientation matters a lot here. The arrangement of the wheels should show an 'X' shape when looked from above or below.
- NRF24L01 PA LNA Module: A 2.4GHz transceiver chip by Nordic Semiconductor. One module is placed here, the other on the ESP32 Transmitter.
- 28BYJ-48 Stepper + ULN2003 Driver: The stepper motor was included for precise back and forth rotating of the head assembly containing the OLED screen and ultrasonic sensor. A ULN2003 was included for the suitable power step-up.
- SG90 Servo Motor: The motor for the kicker, which can rotate in precise angles and is controlled by PWM signals. It controls the small paddle at the front of      the robot.
- HC-SR04 Ultrasonic sensor: The primary sensor used in this bot. If the sensor reads less than 20cm distance from a wall, the code stops all motors completely.
- 128×64 I²C OLED: The "face" of the robot. It was added for a good aesthetic, and shows blinking eyes as an animation when in "eyes mode" (an idle animation        inspired by the Anki Cozmo robot) and a "BLOCKED" warning with distance reading when the bot is too close to an obstacle.
- L298N Motor Drivers (x2): H-bridge motor driver chips. The Pico's GPIO pins can't supply enough current to drive motors directly, so the L298N takes logic 
  signals from the Pico and switches a separate higher-current power supply to the motors. Each driver controls 2 motors — one for the left side, one for 
  the right.
  <img width="933" height="696" alt="Screenshot 2026-06-13 125219" src="https://github.com/user-attachments/assets/c1389b7a-4b28-4f33-9319-76f1a5ae95c9" />
### Transmitter Components:-  
- 128×160 TFT (thin film transistor) Screen (ST7735): This is primarily an aesthetic addition to display information about the current status of the bot. The Gruvbox   Dark colour scheme was used, and the dashboard content include the header with "ROBOTCAR" title, joystick position indicator (moving dot in a circle), 4 wheel state boxes (green = spinning forward, red = spinning backward), EYE/KICK button indicators and a scrolling log panel showing last 3 actions.
- PS2 Joystick Module: the primary controller for the bot, read by the ESP32 module. Mapped to ±512 range, center = 0. Values within ±30 of centre are treated as    0 — this prevents tiny joystick wobbles from making the robot slowly creep when not touching it.
- Touch Sensors (TTP223) modules (×2) — Kick and Mode Buttons: capacitive touch sensor modules that use edge detection, only triggers on the rising edge (LOW →      HIGH    transition), not while held. Kick button: Rising edge → sets kickActive flag in ControlData packet. Robot receives this → servo kicks → returns after      400ms. Mode     button: Rising edge → toggles eyesMode boolean. This switches both the robot's OLED display mode AND the transmitter's dashboard button            highlight.
- NRF24L01 PA LNA Module: The second placement of the transceiver module.
<img width="940" height="415" alt="Screenshot 2026-06-13 125251" src="https://github.com/user-attachments/assets/6948728c-7223-461d-b0d2-91c07961fce2" />
# Errors/Problems Faced  
### 1. Shared SPI Bus — TFT and NRF24 conflicting  
The TFT screen and NRF24 module both use SPI. In theory, multiple SPI devices can share the same bus with separate CS pins. In practice I could never get 
both working simultaneously on the Pi Pico — one would always interfere with the other.

**Fix:** Switched to the ESP32 for the transmitter, which has two independent hardware SPI buses — HSPI for the TFT, VSPI for the NRF24. No sharing, no conflict.
---
### 2. One-way NRF Communication — couldn't send distance data back  
I wanted to display the robot's live distance reading on the transmitter dashboard. This required the robot to send data back, but the NRF24 operates in either transmit or receive mode — never both simultaneously. The robot's NRF was already in receive mode listening for commands.

**Fix:** No clean fix within the project timeline. The robot handles obstacle stopping autonomously (stops at <20cm) so the distance display on the controller wasn't critical. The proper solution would be using the NRF24's built-in ACK payload feature — attaching the distance reading to the acknowledgement packet sent back during normal TX/RX handshake — but this wasn't implemented in time.

# Code description

### Architecture
Two separate programs — one on the Pi Pico (robot), one on the ESP32 (transmitter). They communicate via a shared `ControlData` struct sent over NRF24 radio.

### Robot Car (Pi Pico)
- `RobotCar.ino` — main loop
- `Pins.h` — all GPIO assignments
- `Motors.h` — mecanum drive logic
- `Stepper28BYJ.h` — non-blocking head sweep
- `Ultrasonic.h` — distance sensing + obstacle stop
- `Eyes.h` — wrapper for playfultechnology eyes library

### Transmitter (ESP32)
- `Transmitter.ino` — main loop
- `Colours.h` — gruvbox dark RGB565 palette
- `DrawUI.h` — full TFT dashboard (joystick, wheels, log, buttons)
- `User_Setup.h` — TFT_eSPI config (see Dependencies)

## Dependencies and Notes

### Libraries
| Library | Install |
|---|---|
| RF24 by TMRh20 | `arduino-cli lib install "RF24"` |
| Adafruit SSD1306 | `arduino-cli lib install "Adafruit SSD1306"` |
| Adafruit GFX | `arduino-cli lib install "Adafruit GFX Library"` |
| TFT_eSPI by Bodmer | `arduino-cli lib install "TFT_eSPI"` |
| esp32-eyes by playfultechnology | manual — https://github.com/playfultechnology/esp32-eyes |

### Notes
- **User_Setup.h** — `Transmitter/User_Setup.h` must be copied into your TFT_eSPI library folder, replacing the existing one to configure the HSPI and correct pins.

- **esp32-eyes** — not on the Arduino library manager. Download the ZIP from the link above and install via `arduino-cli lib install --zip-file esp32-eyes-main.zip`. The library's animated eyes code was written by playfultechnology — `Eyes.h` in this repo is only a thin wrapper around it. Credit goes to playfultechonology for the library.
