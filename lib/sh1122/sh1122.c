#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "sh1122.h"

// Frame buffer
static uint8_t FrameBuffer[OLED_WIDTH][OLED_HEIGHT] = {0};

struct Gray_16_Color Display_Color = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
//----------------------------------------------------------------------------------------
// Low Level Functions
//----------------------------------------------------------------------------------------

// Send one-byte command to SH1122
static void SendOneByteCommand(uint8_t Cmd)
{
    HAL_GPIO_WritePin(OLED_nCS_GPIO_Port, OLED_nCS_Pin, GPIO_PIN_RESET); // Select OLED
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);   // Command mode
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Cmd, 1, HAL_MAX_DELAY);        // Send byte
    HAL_GPIO_WritePin(OLED_nCS_GPIO_Port, OLED_nCS_Pin, GPIO_PIN_SET); // Deselect OLED
}

// Send double-byte command to SH1122
static void SendDoubleByteCommand(uint8_t CmdH, uint8_t CmdL)
{
    HAL_GPIO_WritePin(OLED_nCS_GPIO_Port, OLED_nCS_Pin, GPIO_PIN_RESET); // Select OLED
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);   // Command mode
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&CmdH, 1, HAL_MAX_DELAY);        // Send byte 1
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&CmdL, 1, HAL_MAX_DELAY);        // Send byte 2
    HAL_GPIO_WritePin(OLED_nCS_GPIO_Port, OLED_nCS_Pin, GPIO_PIN_SET); // Deselect OLED
}

//----------------------------------------------------------------------------------------
// SH1122 Control Functions
//----------------------------------------------------------------------------------------

// 1 and 2: Set lower (00H - 0FH) and higher (10H - 17H) column address of display RAM
void SH1122_SetColumnAddress(uint8_t Value)
{
    if (Value > 0x7F)
        return;

    uint8_t CmdH = (Value >> 4) + 0x10;
    uint8_t CmdL = Value & 0x0F;
    SendDoubleByteCommand(CmdH, CmdL);
}

// 3-5: Blank

// 6: Set display start line: (40H - 7FH)
void SH1122_SetDisplayStartLine(uint8_t Value)
{
    if (Value > 0x3F)
        return;

    uint8_t Cmd = Value + 0x40;
    SendOneByteCommand(Cmd);
}

// 7: Set contrast control register
void SH1122_SetContrast(uint8_t Value)
{
    uint8_t Cmd = 0x81;
    SendDoubleByteCommand(Cmd, Value);
}

// 8: Set segment re-map: (A0H - A1H)
static void SH1122_SetSegmentRemap(uint8_t Value)
{
    uint8_t Cmd = Value + 0xA0;
    SendOneByteCommand(Cmd);
}

// 9: Set entire display OFF/ON: (A4H - A5H)
void SH1122_EntireDisplay(uint8_t State)
{
    uint8_t Cmd;
    (State == 1) ? (Cmd = 0xA5) : (Cmd = 0xA4);
    SendOneByteCommand(Cmd);
}

// 10: Set normal/reverse display: (A6H - A7H)
void SH1122_DisplayMode(uint8_t State)
{
    uint8_t Cmd;
    (State == 1) ? (Cmd = 0xA7) : (Cmd = 0xA6);
    SendOneByteCommand(Cmd);
}

// 11: Set multiplex ration
void SH1122_SetMultiplexRation(uint8_t Value)
{
    if (Value > 0x3F)
        return;

    uint8_t Cmd = 0xA8;
    SendDoubleByteCommand(Cmd, Value);
}

// 12: DC-DC Setting: (Double Bytes Command)
// Not implemented

// 13: Display power ON/OFF
void SH1122_DisplayPower(uint8_t State)
{
    uint8_t Cmd;
    (State == 1) ? (Cmd = 0xAF) : (Cmd = 0xAE);
    SendOneByteCommand(Cmd);
}

// 14: Set row address of display RAM
static void SH1122_SetRowAddress(uint8_t Value)
{
    if (Value > 0x3F)
        return;

    uint8_t Cmd = 0xB0;
    SendDoubleByteCommand(Cmd, Value);
}

// 15: Set common output scan direction: (C0H - C8H)
static void SH1122_SetScanDirection(uint8_t Direction)
{
    uint8_t Cmd;
    (Direction == 0) ? (Cmd = 0xC0) : (Cmd = 0xC8);
    SendOneByteCommand(Cmd);
}

// 16: Set display offset
void SH1122_SetDisplayOffset(uint8_t Value)
{
    if (Value > 0x3F)
        return;

    uint8_t Cmd = 0xD3;
    SendDoubleByteCommand(Cmd, Value);
}

// 17: Set Display Clock Divide Ratio/Oscillator Frequency: (Double Bytes Command)
static void SH1122_SetDisplayClock(uint8_t Divider, uint8_t Frequency)
{
    if ((Divider > 0x0F) || (Frequency > 0x0F))
        return;

    uint8_t Cmd = 0xD5;
    uint8_t Value = (Frequency << 4) & Divider;
    SendDoubleByteCommand(Cmd, Value);
}

// 18: Set Discharge/Precharge Period: (Double Bytes Command)
static void SH1122_SetDisPreChargePeriod(uint8_t Value)
{
    uint8_t Cmd = 0xD9;
    SendDoubleByteCommand(Cmd, Value);
}

// 19: Set VCOM Deselect Level: (Double Bytes Command)
void SH1122_VCOMDeselectLevel(uint8_t Value)
{
    uint8_t Cmd = 0xDB;
    SendDoubleByteCommand(Cmd, Value);
}

// 20: Set VSEGM Level: (Double Bytes Command)
void SH1122_VSEGMLevel(uint8_t Value)
{
    uint8_t Cmd = 0xDC;
    SendDoubleByteCommand(Cmd, Value);
}

// 21: Set Discharge VSL Level (30H - 3FH)
void SH1122_SetDischargeVSLLevel(uint8_t Value)
{
    if (Value > 0x0F)
        return;

    uint8_t Cmd = Value + 0x30;
    SendOneByteCommand(Cmd);
}

// 25: Write display data
void SH1122_WriteData(uint8_t *pData, uint32_t DataLen)
{
    HAL_GPIO_WritePin(OLED_nCS_GPIO_Port, OLED_nCS_Pin, GPIO_PIN_RESET); // Select OLED
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);   // Data mode
    HAL_SPI_Transmit(&hspi1, (uint8_t *)pData, DataLen, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(OLED_nCS_GPIO_Port, OLED_nCS_Pin, GPIO_PIN_SET); // Deselect OLED
}

// Reset display
static void SH1122_Reset(void)
{
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
}

// Clear display internal RAM
static void SH1122_ClearRAM(void)
{
    uint8_t ScreenBuf[OLED_WIDTH * OLED_HEIGHT / 2] = {0};
    SH1122_WriteData(ScreenBuf, OLED_WIDTH * OLED_HEIGHT / 2);
}

//----------------------------------------------------------------------------------------
// High Level Display Functions
//----------------------------------------------------------------------------------------

// Rotate 180 degrees
void Display_SetOrienation(uint8_t State)
{
    if (State == OLED_DISP_ROTATE180)
    {
        SH1122_SetRowAddress(32);
        SH1122_SetScanDirection(1);
        SH1122_SetSegmentRemap(1);
    }
}

// Init display
void Display_Init(void)
{
    SH1122_Reset();

    // Write your custom initialization code here

    SH1122_ClearRAM();
    SH1122_DisplayPower(OLED_POWER_ON);
    HAL_Delay(100);
}

// Update display
void Display_SendFrame(void)
{
    uint8_t Buffer[OLED_WIDTH * OLED_HEIGHT / 2] = {0};

    for (uint32_t i = 0; i < OLED_HEIGHT; i++)
    {
        for (uint32_t j = 0; j < OLED_WIDTH - 1; j = j + 2)
        {
            Buffer[(j + i * OLED_WIDTH) / 2] = (FrameBuffer[j][i] & 0xF0) | ((FrameBuffer[j + 1][i]) & 0x0F);
        }
    }

    SH1122_WriteData(Buffer, OLED_WIDTH * OLED_HEIGHT / 2);
}

// Draw a pixel in (x, y) coordinates
void Frame_DrawPixel(uint16_t x, uint16_t y, uint8_t color)
{
    if ((x >= OLED_WIDTH) || (y >= OLED_HEIGHT))
        return;

    FrameBuffer[x][y] = color;
}

// Draw a line from (x1, y1) to (x2, y2)
void Frame_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    const int16_t deltaX = abs(x2 - x1);
    const int16_t deltaY = abs(y2 - y1);
    const int16_t signX = x1 < x2 ? 1 : -1;
    const int16_t signY = y1 < y2 ? 1 : -1;

    int16_t error = deltaX - deltaY;

    Frame_DrawPixel(x2, y2, color);

    while (x1 != x2 || y1 != y2)
    {
        Frame_DrawPixel(x1, y1, color);
        const int16_t error2 = error * 2;

        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}

// Draw rectangle
void Frame_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    Frame_DrawLine(x1, y1, x1, y2, color);
    Frame_DrawLine(x2, y1, x2, y2, color);
    Frame_DrawLine(x1, y1, x2, y1, color);
    Frame_DrawLine(x1, y2, x2, y2, color);
}

// Draw rectangle filled
void Frame_DrawRectangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t bColor, uint8_t fColor)
{
    if (x1 > x2)
    {
        int16_t Temp = x1;
        x1 = x2;
        x2 = Temp;
    }
    
    if (y1 > y2)
    {
        int16_t Temp = y1;
        y1 = y2;
        y2 = Temp;
    }

    // Fill
    int16_t y_start = y1;

    while (y_start <= y2)
    {
        Frame_DrawLine(x1, y_start, x2, y_start, fColor);
        y_start++;
    }

    // Border
    Frame_DrawLine(x1, y1, x1, y2, bColor);
    Frame_DrawLine(x2, y1, x2, y2, bColor);
    Frame_DrawLine(x1, y1, x2, y1, bColor);
    Frame_DrawLine(x1, y2, x2, y2, bColor);
}

// Draw circle
void Frame_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint8_t color)
{
    int x = 0;
    int y = radius;
    int delta = 1 - 2 * radius;
    int error = 0;

    while (y >= 0)
    {
        Frame_DrawPixel(x0 + x, y0 + y, color);
        Frame_DrawPixel(x0 + x, y0 - y, color);
        Frame_DrawPixel(x0 - x, y0 + y, color);
        Frame_DrawPixel(x0 - x, y0 - y, color);
        error = 2 * (delta + y) - 1;

        if (delta < 0 && error <= 0)
        {
            ++x;
            delta += 2 * x + 1;
            continue;
        }

        error = 2 * (delta - x) - 1;

        if (delta > 0 && error > 0)
        {
            --y;
            delta += 1 - 2 * y;
            continue;
        }

        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

// Draw symbol
uint8_t Frame_DrawChar(uint16_t X, uint16_t Y, uint8_t FontID, uint8_t Char, uint8_t color)
{
    // Указатель на подтабличку конкретного символа шрифта
    uint8_t *pCharTable = font_GetFontStruct(FontID, Char);
    uint8_t CharWidth = font_GetCharWidth(pCharTable);   // Ширина символа
    uint8_t CharHeight = font_GetCharHeight(pCharTable); // Высота символа
    pCharTable += 2;

    if (FontID == FONTID_6X8M)
    {
        for (uint8_t row = 0; row < CharHeight; row++)
        {
            for (uint8_t col = 0; col < CharWidth; col++)
            {
                if (pCharTable[row] & (1 << (7 - col))) Frame_DrawPixel(X + col, Y + row, color);
            }
        }
    }
    else
    {
        for (uint8_t row = 0; row < CharHeight; row++)
        {
            for (uint8_t col = 0; col < CharWidth; col++)
            {
                if (col < 8)
                {
                    if (pCharTable[row * 2] & (1 << (7 - col)))
                        Frame_DrawPixel(X + col, Y + row, color);
                }
                else
                {
                    if (pCharTable[row * 2 + 1] & (1 << (15 - col)))
                        Frame_DrawPixel(X + col, Y + row, color);
                }
            }
        }
    }
    

    return CharWidth;
}

int16_t Frame_getFullStrWidth(uint8_t FontID, char *Str)
{
    uint8_t done = 0;     // Флаг окончания вывода
    int16_t StrWidth = 0; // Ширина строки в пикселях

    // Вывод строки
    while (!done)
    {
        switch (*Str)
        {
        case '\0': // Конец строки
            done = 1;
            break;
        case '\n': // Переход на следующую строку
        case '\r': // Переход в начало строки
            break;
        default: // Отображаемый символ
            StrWidth += font_GetCharWidth(font_GetFontStruct(FontID, *Str));
            break;
        }
        Str++;
    }

    return StrWidth;
}

int16_t Frame_getStrWidth(uint8_t FontID, char *Str)
{
    uint8_t done = 0;     // Флаг окончания вывода
    int16_t StrWidth = 0; // Ширина строки в пикселях
    int16_t StrMaxWidth = 0;
    uint8_t CarryFlag = 0; // Флаг переноса

    // Вывод строки
    while (!done)
    {
        switch (*Str)
        {
            case '\0': // Конец строки
                if (!CarryFlag)
                    StrWidth = StrMaxWidth; //
                done = 1;
                break;
            case '\n': // Переход на следующую строку
            case '\r': // Переход в начало строки
                CarryFlag = 1;
                if (StrMaxWidth > StrWidth)
                {
                    // Нашли кусок строки длиннее
                    StrWidth = StrMaxWidth;
                    StrMaxWidth = 0;
                }
                break;
            default: // Отображаемый символ
                StrMaxWidth += font_GetCharWidth(font_GetFontStruct(FontID, *Str));
                break;
        }
        Str++;
    }

    return StrWidth;
}

int16_t Frame_getFormatStrWidth(uint8_t FontID, const char *args, ...)
{
    char StrBuff[256];

    va_list ap;
    va_start(ap, args);
    vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
    va_end(ap);

    return Frame_getFullStrWidth(FontID, StrBuff);
}

// Draw string
int16_t Frame_DrawString(uint16_t X, uint16_t Y, uint8_t FontID, uint8_t *Str, uint8_t hAlign, uint8_t color)
{
    uint8_t done = 0;      // Флаг окончания вывода
    uint8_t StrHeight = 8; // Высота символов в пикселях для перехода на следующую строку
    uint8_t *SubStr = Str; // Определяем начало подстроки
    uint8_t SubStrWidth = 0; // Ширина подстроки
    int16_t Xstart = X;    // Куда будем переводить каретку при переходе на новую строку

    // Подсчет максимальной ширины подстроки
    while (!done)
    {
        switch (*SubStr)
        {
            case '\0': // Конец строки
                done = 1;
                break;
            case '\n': // Переход на следующую строку
                // Нашли перенос!
                done = 1;
                break;
            case '\r': // Переход в начало строки
                break;
            default: // Отображаемый символ
                SubStrWidth += font_GetCharWidth(font_GetFontStruct(FontID, *SubStr));
                break;
        }
        SubStr++;
    }
    done = 0;

    if (hAlign == RIGHT)
    {
        X -= SubStrWidth;
    }
    else if (hAlign == CENTER)
    {
        X -= SubStrWidth / 2;
    }

    // Вывод строки
    while (!done)
    {
        switch (*Str)
        {
            case '\0': // Конец строки
                done = 1;
                break;
            case '\n': // Переход на следующую строку
                Y += StrHeight;
                Frame_DrawString(Xstart, Y, FontID, Str + 1, hAlign, color);
                done = 1;
                break;
            case '\r': // Переход в начало строки
                break;
            default: // Отображаемый символ
                X += Frame_DrawChar(X, Y, FontID, *Str, color);
                StrHeight = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
                break;
        }
        Str++;
    }

    return X;
}

// Draw format string
int16_t Frame_printf(uint16_t X, uint16_t Y, uint8_t FontID, uint8_t color, uint8_t hAlign, uint8_t vAlign, const char *args, ...)
{
    char StrBuff[100];

    va_list ap;
    va_start(ap, args);
    vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
    va_end(ap);

    // Подсчет количества переносов
    uint8_t done = 0; // Флаг окончания 
    uint8_t *TempStr = StrBuff;
    uint8_t carries = 0;
    while (!done)
    {
        switch (*TempStr)
        {
        case '\0': // Конец строки
            done = 1;
            break;
        case '\n': // Переход на следующую строку
            // Нашли перенос!
            carries++;
            break;
        case '\r': // Переход в начало строки
            break;
        default: // Отображаемый символ
            break;
        }
        TempStr++;
    }
    
    if (vAlign == BOTTOM)
    {
        Y -= font_GetCharHeight(font_GetFontStruct(FontID, *StrBuff)) * (carries + 1);
    }
    else if (vAlign == CENTER)
    {
        Y -= font_GetCharHeight(font_GetFontStruct(FontID, *StrBuff)) * (carries + 1) / 2;
    }

    return Frame_DrawString(X, Y, FontID, (uint8_t *)StrBuff, hAlign, color);
}
