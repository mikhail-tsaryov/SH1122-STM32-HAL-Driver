#ifndef _F16X26F_H
#define _F16X26F_H

#include "main.h"

#define f16x26_FLOAT_WIDTH 16
#define f16x26_FLOAT_HEIGHT 26

// Кол-во символов в таблице шрифта
#define f16x26f_NOFCHARS 256

// Функция возвращает указатель на подтаблицу символа Char
uint8_t *f16x26f_GetCharTable(uint8_t Char);

#endif
