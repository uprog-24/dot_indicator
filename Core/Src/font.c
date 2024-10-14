/**
 * @file font.c
 */
#include "font.h"
#include "tim.h"
#include "dot.h"

#include <string.h>
#include <stdio.h>

#define FONT_WIDTH 5U ///< Width of font.
#define FONT_HEIGHT 8U ///< Height of font.
#define MAX_CHARS_ON_SCREEN 2U ///< Limit number chars on screen.
#define SCREEN_WIDTH (MAX_CHARS_ON_SCREEN * FONT_WIDTH) ///< Width of screen.
#define SCREEN_HEIGHT FONT_HEIGHT ///< Height of screen.
#define EMPTY_SYMBOL_IDX 10U ///< Index of Clear symbol.

#define BINARY_SYMBOL_SIZE 8 ///< 8 bit massive for binary representation of symbol.
#define START_BIT_INDEX_FONT 6

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
				0b01100000, 0b01010000, 0b01001000, 0b00000000 } }

};

/**
 * @brief  Convert digit representation of symbol to binary.
 * @param  number
 * @param  *binary_mas Binary representation of the symbol
 * @retval None
 */
static void convert_number_from_10_to_2(uint8_t number, uint8_t *binary_mas) {
	int8_t font_index = START_BIT_INDEX_FONT;
	while (font_index != -1) {
		binary_mas[font_index] = number % 2;
		number /= 2;
		font_index--;
	}
}

/**
 * @brief  Find binary representation of the symbol.
 * @param  symbol
 * @retval uint8_t* Pointer to binary_symbol
 */
static uint8_t* find_binary_representation(char symbol) {
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

/**
 * @brief  Set a symbol to matrix.
 * @param  symbol
 * @param  pos Start position (index of column) for symbol
 * @retval None
 */
void set_symbol_on_matrix(char symbol, uint8_t pos, uint8_t shift) {
	uint8_t *cur_item = NULL;

	cur_item = find_binary_representation(symbol);

	if (cur_item == NULL) {
		return;
	}

	for (uint8_t cur_row = 0; cur_row < ROWS; cur_row++) {

		for (uint8_t row = 0; row < ROWS; row++) {
			if (row == cur_row) {
				set_row_state(cur_row, TURN_ON);
			} else {
				set_row_state(row, TURN_OFF);
			}
		}

		uint8_t binary_number[8];
		uint8_t num_bit = 0;

		if (cur_row + shift < 8) {
			convert_number_from_10_to_2(cur_item[cur_row + shift],
					binary_number);
		} else {
			memset(binary_number, 0, 8);
		}

		uint8_t start_col = pos;
		uint8_t end_col = start_col + FONT_WIDTH;
		for (uint8_t col = start_col; col < end_col; col++) {
			if (binary_number[num_bit] == 1) {
				set_col_state(col, TURN_ON);
			} else {
				set_col_state(col, TURN_OFF);
			}
			num_bit++;
		}
		TIM3_Delay_us(600);
		set_all_cols_state(TURN_OFF);
	}

}

/**
 * @brief  Set symbols to matrix.
 * @param  *str_symbols String to be set on matrix
 * @retval None
 */
void set_symbols_to_matrix(char *str_symbols) {
	uint8_t cur_pos = 1;

	uint8_t ind = 0;
	while (ind < MAX_CHARS_ON_SCREEN) {
		set_symbol_on_matrix(*str_symbols, cur_pos, 0);
		cur_pos = cur_pos + BINARY_SYMBOL_SIZE;
		str_symbols++;
		ind++;
	}
}

