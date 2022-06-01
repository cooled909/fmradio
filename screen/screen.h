#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8


#define SerialDebugging true

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

uint16_t        Display_Text_Color         = Display_Color_Green;
uint16_t        Display_Background_Color    = Display_Color_Black;

bool            isDisplayVisible           = false;

float           freq                       = 96.7;

const size_t MaxString = 16;

char oldTimeString[MaxString] = { 0 };

void drawStereo();