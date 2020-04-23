#ifndef __SH1122_H
#define __SH1122_H

#include "spi.h"
#include <stdio.h> 
#include "font.h"

// Display size
#define OLED_WIDTH 256
#define OLED_HEIGHT 64

#define OLED_POWER_OFF 0
#define OLED_POWER_ON 1

#define OLED_ENTIRE_OFF 0
#define OLED_ENTIRE_ON 1

#define OLED_MODE_NORMAL 0
#define OLED_MODE_REVERSE 1

#define OLED_SCAN_NORMAL 0
#define OLED_SCAN_REVERSE 1

#define OLED_DISP_NORMAL 0
#define OLED_DISP_ROTATE180 1

#define LEFT 0
#define RIGHT 1
#define CENTER 2
#define TOP 3
#define BOTTOM 4

struct Gray_16_Color
{
    uint8_t Gray_00; 
    uint8_t Gray_01;
    uint8_t Gray_02;
    uint8_t Gray_03;
    uint8_t Gray_04;
    uint8_t Gray_05;
    uint8_t Gray_06;
    uint8_t Gray_07;
    uint8_t Gray_08;
    uint8_t Gray_09;
    uint8_t Gray_10;
    uint8_t Gray_11;
    uint8_t Gray_12;
    uint8_t Gray_13;
    uint8_t Gray_14;
    uint8_t Gray_15;
};

extern struct Gray_16_Color Display_Color;
//----------------------------------------------------------------------------------------
// SH1122 Control Functions
//----------------------------------------------------------------------------------------

// 1 and 2: Set Lower (00H - 0FH) and Higher (10H - 17H) Column Address of display RAM
void SH1122_SetColumnAddress(uint8_t Value);
// 6: Set Display Start Line: (40H - 7FH)
void SH1122_SetDisplayStartLine(uint8_t Value);
// 7: Set Contrast Control Register
void SH1122_SetContrast(uint8_t Value);
// 9: Set Entire Display OFF/ON: (A4H - A5H)
void SH1122_EntireDisplay(uint8_t State);
// 10: Set Normal/Reverse Display: (A6H - A7H)
void SH1122_DisplayMode(uint8_t State);
// 11: Set Multiplex Ration
void SH1122_SetMultiplexRation(uint8_t Value);
// 13: Display Power Control
void SH1122_DisplayPower(uint8_t State);
// 16: Set Display Offset
void SH1122_SetDisplayOffset(uint8_t Value);
// 25: Write Display Data
void SH1122_WriteData(uint8_t *pData, uint32_t DataLen);

//----------------------------------------------------------------------------------------
// High Level Display Functions
//----------------------------------------------------------------------------------------

// Rotate 180 degrees
void Display_SetOrienation(uint8_t State);
// Init display
void Display_Init();
// Update display
void Display_SendFrame(void);
// Draw a pixel in (x, y) coordinates
void Frame_DrawPixel(uint16_t x, uint16_t y, uint8_t color);
// Draw a line from (x1, y1) to (x2, y2)
void Frame_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
// Draw rectangle
void Frame_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
// Draw rectangle filled
void Frame_DrawRectangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t bColor, uint8_t fColor);
// Draw circle
void Frame_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint8_t color);
// Draw symbol
uint8_t Frame_DrawChar(uint16_t X, uint16_t Y, uint8_t FontID, uint8_t Char, uint8_t color);
// Draw string
int16_t Frame_DrawString(uint16_t X, uint16_t Y, uint8_t FontID, uint8_t *Str, uint8_t hAlign, uint8_t color);
// Draw formatted string
int16_t Frame_printf(uint16_t X, uint16_t Y, uint8_t FontID, uint8_t color, uint8_t hAlign, uint8_t vAlign, const char *args, ...);

#endif