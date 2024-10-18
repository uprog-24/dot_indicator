/**
 * @file font.c
 */
#include "font.h"
#include "tim.h"
#include "dot.h"

#include <string.h>
#include <stdio.h>

#define START_BIT_INDEX_FONT 6 ///< Start index for element of symbols[]

/**
 * Stores the parameters of the symbol: symbol and binary_symbol massive
 */
typedef struct {
	char symbol;
	uint8_t binary_symbol[8];
} symbol_t;

/// Massive of symbols
static symbol_t symbols[] = { { 'c', { // clear
		0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
				0b00000000, 0b00000000 } }, { '>', { // up
		0b00010000, 0b00111000, 0b01010100, 0b00010000, 0b00010000, 0b00010000,
				0b00010000, 0b00010000 }

}, { '<', { // down
		0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b01010100,
				0b00111000, 0b00010000 }

}, { '0', { 0b00110000, 0b01001000, 0b01001000, 0b01001000, 0b01001000,
		0b01001000, 0b01001000, 0b00110000 } },
		{ '1', { 0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b00100000,
				0b00100000, 0b00100000, 0b01110000 } }, { '2', { 0b00110000,
				0b01001000, 0b00001000, 0b00001000, 0b00010000, 0b00100000,
				0b01000000, 0b01111000 } }, { '3', { 0b01111000, 0b00001000,
				0b00010000, 0b00110000, 0b00001000, 0b00001000, 0b01001000,
				0b00110000 } }, { '4', { 0b00001000, 0b00011000, 0b00101000,
				0b01001000, 0b01111000, 0b00001000, 0b00001000, 0b00001000 } },
		{ '5', { 0b01111000, 0b01000000, 0b01000000, 0b01110000, 0b00001000,
				0b00001000, 0b01001000, 0b00110000 } }, { '6', { 0b00110000,
				0b01001000, 0b01000000, 0b01110000, 0b01001000, 0b01001000,
				0b01001000, 0b00110000 } }, { '7', { 0b01111000, 0b00001000,
				0b00001000, 0b00010000, 0b00100000, 0b00100000, 0b00100000,
				0b00100000 } }, { '8', { 0b00110000, 0b01001000, 0b01001000,
				0b00110000, 0b01001000, 0b01001000, 0b01001000, 0b00110000 } },
		{ '9', { 0b00110000, 0b01001000, 0b01001000, 0b01001000, 0b00111000,
				0b00001000, 0b01001000, 0b00110000 } },

		{ 'O', {

		0b00000000, 0b00110000, 0b01001000, 0b01001000, 0b01001000, 0b01001000,
				0b00110000, 0b00000000

		} }, { 'K', { 0b00000000, 0b01001000, 0b01010000, 0b01100000,
				0b01100000, 0b01010000, 0b01001000, 0b00000000 } }, { '-', {
				0b00000000, 0b00000000, 0b00000000, 0b01110000, 0b00000000,
				0b00000000, 0b00000000, 0b00000000 } }, { 'b', { 0b01000000,
				0b01000000, 0b01000000, 0b01110000, 0b01001000, 0b01001000,
				0b01001000, 0b00110000 } }, { 'L', { 0b01000000, 0b01000000,
				0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01001000,
				0b01111000 } }, { 'A', { 0b00110000, 0b01001000, 0b01001000,
				0b01111000, 0b01001000, 0b01001000, 0b01001000, 0b01001000 } },
		{ 'P', { 0b01110000, 0b01001000, 0b01001000, 0b01001000, 0b01110000,
				0b01000000, 0b01000000, 0b01000000 } }, { 'H', { 0b01001000,
				0b01001000, 0b01001000, 0b01111000, 0b01001000, 0b01001000,
				0b01001000, 0b01001000 } }, { 'C', { 0b00110000, 0b01001000,
				0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01001000,
				0b00110000 } }, { 'E', { 0b01111000, 0b01000000, 0b01000000,
				0b01000000, 0b01111000, 0b01000000, 0b01000000, 0b01111000 } },
		{ 'F', { 0b01111000, 0b01000000, 0b01000000, 0b01000000, 0b01110000,
				0b01000000, 0b01000000, 0b01000000 } }, { 'U', { 0b01001000,
				0b01001000, 0b01001000, 0b01001000, 0b01001000, 0b01001000,
				0b01001000, 0b00110000 } }, { 'p', // П
				{ 0b01111000, 0b01001000, 0b01001000, 0b01001000, 0b01001000,
						0b01001000, 0b01001000, 0b01001000 } }

};

/**
 * @brief  Convert decimal number to binary.
 * @param  number
 * @param  *binary_mas Binary representation of the symbol
 * @retval None
 */
void convert_number_from_dec_to_bin(uint8_t number, uint8_t *binary_mas,
		uint8_t bin_size) {
	int8_t font_index = bin_size;
	uint8_t bit_num = 0;
	while (font_index != -1) {
		binary_mas[bit_num] = number % 2;
		number /= 2;
		font_index--;
		bit_num++;
	}
}

/**
 * @brief  Find binary representation of the symbol.
 * @param  symbol
 * @retval uint8_t* Pointer to binary_symbol
 */
uint8_t* find_binary_representation(char symbol) {
	for (uint8_t ind = 0; ind < sizeof(symbols) / sizeof(symbols[0]); ind++) {
		if (symbols[ind].symbol == symbol) {
			return symbols[ind].binary_symbol;
		}
	}
	return NULL;
}

/**
 * @brief  Convert integer number (0..9) to char ('0'..'9').
 * @param  number Number that will be converted into char.
 * @retval char
 */
char convert_int_to_char(uint8_t number) {
	return number + '0';
}

