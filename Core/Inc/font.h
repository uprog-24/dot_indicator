/**
 * @file    font.h
 * @brief   This file contains all the function prototypes for
 *          the font.c file
 */
#ifndef __FONT_H__
#define __FONT_H__

#include "main.h"
#include <stdint.h>

/**
 * @brief  Set a symbol to matrix.
 * @param  symbol
 * @param  pos Start position (index of column) for symbol
 * @retval None
 */
void set_symbol_on_matrix(char symbol, uint8_t pos, uint8_t shift);

/**
 * @brief  Set symbols to matrix.
 * @param  *str_symbols String to be set on matrix
 * @retval None
 */
void set_symbols_to_matrix(char *str_symbols);

char convert_int_to_char(uint8_t number);

#endif /*__FONT_H__ */
