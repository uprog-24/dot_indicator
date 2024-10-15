/**
 * @file drawing.c
 */
#include "drawing.h"
#include "dot.h"
#include "font.h"
#include "tim.h"

#include <string.h>

#define MAX_CHARS_ON_MATRIX 2U ///< Limit number chars on matrix.

/**
 * @brief  Setting symbols for current floor.
 * @param  *str Pointer to the output string with symbols for the floor.
 * @param  current_floor Number of the current floor.
 * @param  direction Direction of the movement of lift: UPWARD, DOWNWARD, STOP.
 * @retval None
 */
void setting_symbols_floor(char *str, uint8_t current_floor,
		direction_t direction) {

	switch (direction) {
	case UPWARD:
		str[0] = '>';
		break;
	case DOWNWARD:
		str[0] = '<';
		break;
	case STOP:
		str[0] = 'c';
		break;
	}

	if (current_floor > 0 && current_floor < 10) {
		str[1] = convert_int_to_char(current_floor % 10);
		str[2] = 'c';
	} else {
		str[1] = convert_int_to_char(current_floor / 10);
		str[2] = convert_int_to_char(current_floor % 10);
	}
}

/**
 * @brief  Draw a symbol on matrix.
 * @param  symbol
 * @param  pos Start position (index of column) for symbol
 * @retval None
 */
void draw_symbol_on_matrix(char symbol, uint8_t pos, uint8_t shift) {
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
			convert_number_from_dec_to_bin(cur_item[cur_row + shift],
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
 * @brief  Draw string on matrix.
 * @param  *str_symbols String to be set on matrix
 * @retval None
 */
void draw_str_on_matrix(char *str_symbols) {
	uint8_t cur_pos = 1;

	uint8_t ind = 0;
	while (ind < MAX_CHARS_ON_MATRIX) {
		draw_symbol_on_matrix(*str_symbols, cur_pos, 0);
		cur_pos = cur_pos + BINARY_SYMBOL_SIZE;
		str_symbols++;
		ind++;
	}
}
