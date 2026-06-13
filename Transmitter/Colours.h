#pragma once

#define RGB565(r,g,b) ((uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)))

// Backgrounds
#define GRV_BG       RGB565(40,  40,  40)
#define GRV_BG1      RGB565(60,  56,  54)
#define GRV_BG2      RGB565(80,  73,  69)
#define GRV_BG3      RGB565(102, 92,  84)
#define GRV_BG4      RGB565(124, 111, 100)

// Foregrounds
#define GRV_FG       RGB565(235, 219, 178)
#define GRV_FG1      RGB565(213, 196, 161)
#define GRV_FG2      RGB565(189, 174, 147)
#define GRV_FG3      RGB565(168, 153, 132)

// Accents
#define GRV_RED      RGB565(204, 36,  29)
#define GRV_RED_B    RGB565(251, 73,  52)
#define GRV_GREEN    RGB565(152, 151, 26)
#define GRV_GREEN_B  RGB565(184, 187, 38)
#define GRV_YELLOW   RGB565(215, 153, 33)
#define GRV_YELLOW_B RGB565(250, 189, 47)
#define GRV_BLUE     RGB565(69,  133, 136)
#define GRV_BLUE_B   RGB565(131, 165, 152)
#define GRV_PURPLE   RGB565(177, 98,  134)
#define GRV_AQUA     RGB565(104, 157, 106)
#define GRV_AQUA_B   RGB565(142, 192, 124)
#define GRV_ORANGE   RGB565(214, 93,  14)
#define GRV_ORANGE_B RGB565(254, 128, 25)
