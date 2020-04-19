﻿//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "font.h"
#include "f6x8m.h"
#include "f10x16f.h"
#include "f16x26f.h"

// Таблица с указателями на функции извлечения таблицы символа шрифта. Шрифт пока один
const t_font_getchar font_table_funcs[] = 
{
  f6x8m_GetCharTable,
  f10x16f_GetCharTable,
  f16x26f_GetCharTable
};


//==============================================================================
// Функция возвращает указатель на структуру, описывающую символ Char
//==============================================================================
uint8_t *font_GetFontStruct(uint8_t FontID, uint8_t Char)
{
  return font_table_funcs[FontID](Char);
}
//==============================================================================


//==============================================================================
// Функция возвращает ширину символа
//==============================================================================
uint8_t font_GetCharWidth(uint8_t *pCharTable)
{
  return *pCharTable;  // Ширина символа
}
//==============================================================================


//==============================================================================
// Функция возвращает высоту символа
//==============================================================================
uint8_t font_GetCharHeight(uint8_t *pCharTable)
{
  pCharTable++;
  return *pCharTable;  // Высота символа
}
//==============================================================================
