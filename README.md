# mecanum-sensing-bot
I had built this bot originally for the purpose of playing soccer - hence its body and sensing functions are intended to efficiently move and "kick" the ball, and it's also why I included a joystick system as a remote control instead of plain wi-fi. The mecanum wheels help in omni-directional movement. However, this project became my kickstart into the field of signal processing, communication and electronic circuits.
# Bot Description
The robot is made of two separate systems that communicate via NRF24L01 Radio modules operating at 2.4Ghz: the robot car, and the transmimtter. The main microcontroller used in the robot as the brain is the Raspberry Pi Pico RP2040, and for the transmitter, it's the ESP32 Devkit V1.  
### Robot Car Components:-  
- Raspberry Pi Pico RP2040: The brain. Code was programmed in Arduino in C++, as I was more comfortable with it than MicroPython.
- BO Motors + Mecanum wheels (x4): Roller orientation matters a lot here. The arrangement of the wheels should show an 'X' shape when looked from above or below.
- NRF24L01 PA LNA Module: A 2.4GHz transciever chip by Nordic Semiconductor. One module is placed here, the other on the ESP32 Transmitter.
- 28BYJ-48 Stepper + ULN2003 Driver: The stepper motor was included for precise back and forth rotating of the OLED screen. A ULN2003 was included for the suitable     power step-up.
- SG90 Servo Motor: The motor for the kicker, which can rotate in precise angles and is controlled by PWM signals. It controls the small paddle at the front of the     robot.
- HC-SR04 Ultrasonic sensor: The primary sensor used in this bot. If the sensor reads less than 20cm distance from a wall, the code stops all motors completely.
- 128×64 I²C OLED: The "face" of the robot. It was added for a good aesthetic, and shows blinking eyes as an animation when in "eyes mode" (an idle animation           inspired by the Anki Cozmo robot) and a "BLOCKED" warning with distance reading when the bot is too close to an obstacle.
  <img width="933" height="696" alt="Screenshot 2026-06-13 125219" src="https://github.com/user-attachments/assets/c1389b7a-4b28-4f33-9319-76f1a5ae95c9" />
### Transmitter Components:-  
- 128×160 TFT (thin film transistor) Screen (ST7735): This is primarily an aesthetic addition to display information about the current status of the bot. The Gruvbox   Dark colour scheme was used, and the dashboard content include the header with "ROBOTCAR" title, joystick position indicator (moving dot in a circle), 4 wheel        state boxes (green = spinning forward, red = spinning backward), EYE/KICK button indicators and a scrolling log panel showing last 3 actions.
- PS2 Joystick Module: the primary controller for the bot, read by the ESP32 module. Mapped to ±512 range, center = 0. Values within ±30 of centre are treated as 0 —   this prevents tiny joystick wobbles from making the robot slowly creep when not touching it.
- Touch Sensors (TTP223) modules (×2) — Kick and Mode Buttons: capacitive touch sensor modules that use edge detection, only triggers on the rising edge (LOW → HIGH    transition), not while held. Kick button: Rising edge → sets kickActive flag in ControlData packet. Robot receives this → servo kicks → returns after 400ms. Mode     button: Rising edge → toggles eyesMode boolean. This switches both the robot's OLED display mode AND the transmitter's dashboard button highlight.
<img width="940" height="415" alt="Screenshot 2026-06-13 125251" src="https://github.com/user-attachments/assets/6948728c-7223-461d-b0d2-91c07961fce2" />
# Errors/Problems Faced

