#pragma once

// i2c (oled)
#define PIN_I2C_SDA   4
#define PIN_I2C_SCL   5

// nrf24 (spi0)
#define PIN_NRF_MISO  16
#define PIN_NRF_MOSI  19
#define PIN_NRF_SCK   18
#define PIN_NRF_CSN   17
#define PIN_NRF_CE    20

// l298n #1 — left motors
#define PIN_M1_IN1   6
#define PIN_M1_IN2   7
#define PIN_M1_ENA   8

#define PIN_M2_IN1   9
#define PIN_M2_IN2   10
#define PIN_M2_ENB   11

// l298n #2 — right motors
#define PIN_M3_IN1   12
#define PIN_M3_IN2   13
#define PIN_M3_ENA   14

#define PIN_M4_IN1   21
#define PIN_M4_IN2   22
#define PIN_M4_ENB   26

// stepper (uln2003)
#define PIN_STEP_IN1  0
#define PIN_STEP_IN2  1
#define PIN_STEP_IN3  2
#define PIN_STEP_IN4  3

// servo
#define PIN_SERVO    15

// hc-sr04
#define PIN_TRIG     27
#define PIN_ECHO     28
