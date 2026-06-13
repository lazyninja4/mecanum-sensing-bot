#pragma once

// driver — uncomment one to match your screen chip
#define ST7735_DRIVER
// #define ILI9163_DRIVER

#define TFT_WIDTH   128
#define TFT_HEIGHT  160

// st7735 tab colour — try GREENTAB, BLACKTAB, REDTAB if colours look wrong
#define ST7735_GREENTAB

// hspi pins
#define TFT_MOSI  13
#define TFT_SCLK  14
#define TFT_CS    15
#define TFT_DC     2
#define TFT_RST   12

#define USE_HSPI_PORT

#define SPI_FREQUENCY      27000000
#define SPI_READ_FREQUENCY 20000000

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT
