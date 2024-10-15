/**
 * @file    font.h
 * @brief   This file contains all the function prototypes for
 *          the font.c file
 */
#ifndef __FONT_H__
#define __FONT_H__

#include "main.h"
#include <stdint.h>

#define FONT_WIDTH 5U ///< Width of font.
#define BINARY_SYMBOL_SIZE 8 ///< 8 bit massive for binary representation of symbol.

/**
 * @brief  Convert digit representation of symbol to binary.
 * @param  number
 * @param  *binary_mas Binary representation of the symbol
 * @retval None
 */
void convert_number_from_dec_to_bin(uint8_t number, uint8_t *binary_mas);

/**
 * @brief  Find binary representation of the symbol.
 * @param  symbol
 * @retval uint8_t* Pointer to binary_symbol
 */
uint8_t* find_binary_representation(char symbol);

/**
 * @brief  Convert integer number (0..9) to char ('0'..'9').
 * @param  number Number that will be converted into char.
 * @retval char
 */
char convert_int_to_char(uint8_t number);

#endif /*__FONT_H__ */
