/**
 * @file drawing.c
 */
#include "drawing.h"
#include "dot.h"
#include "font.h"
#include "tim.h"

#include <string.h>

#define MAX_CHARS_ON_MATRIX 2U ///< Limit number chars on matrix.

typedef enum CODE_FLOOR {
	FLOOR_0 = 0,
	FLOOR_10 = 10,
	FLOOR_40 = 40,
	FLOOR_MINUS_1 = 41,
	FLOOR_MINUS_9 = 49,
	RESERVE = 50,
	SEISMIC_DANGER = 51,
	LIFT_NOT_WORK = 52,
	TRANSFER_FIREFIGHTERS = 53,
	CODE_FLOOR_54 = 54,
	SERVICE = 55,
	EVACUATION = 56,
	FIRE_DANGER = 57,
	FAULT_IBP = 58,
	LOADING = 59
} code_floor_t;

typedef struct {
	code_floor_t code_floor;
	char symbol;
} code_floor_symbol_t;

#define CODE_FLOOR_SYMBOLS_SIZE 10

code_floor_symbol_t code_floor_symbols[] = { { RESERVE, 'b' }, { SEISMIC_DANGER,
		'L' }, { LIFT_NOT_WORK, 'A' }, { TRANSFER_FIREFIGHTERS, 'P' }, {
		CODE_FLOOR_54, 'H' }, { SERVICE, 'C' }, { EVACUATION, 'E' }, {
		FIRE_DANGER, 'F' }, { FAULT_IBP, 'U' }, { LOADING, 'p' } };

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

	if (current_floor >= FLOOR_0 && current_floor < FLOOR_10) {
		str[1] = convert_int_to_char(current_floor % 10);
		str[2] = 'c';
	} else if (current_floor >= FLOOR_10 && current_floor <= FLOOR_40) {
		str[1] = convert_int_to_char(current_floor / 10);
		str[2] = convert_int_to_char(current_floor % 10);
	}

	else if (current_floor >= FLOOR_MINUS_1 && current_floor <= FLOOR_MINUS_9) {
		str[1] = '-';
		str[2] = convert_int_to_char(current_floor % 10);
	} else {

		// special symbols
		for (uint8_t ind = 0; ind < CODE_FLOOR_SYMBOLS_SIZE; ind++) {
			if (code_floor_symbols[ind].code_floor == current_floor) {
				str[1] = code_floor_symbols[ind].symbol;
				str[2] = 'c';
			}
		}

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
		uint8_t num_bit = 6;

		if (cur_row + shift < 8) {
			convert_number_from_dec_to_bin(cur_item[cur_row + shift],
					binary_number, 6);
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
			num_bit--;
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

    // stop floor 1..9: c1c and special symbols: F, P ...
	if (str_symbols[0] == 'c' && str_symbols[2] == 'c') {
		draw_symbol_on_matrix(str_symbols[1], 6, 0);
	} else if (str_symbols[0] == 'c') { // stop floor 10..99: c10 and -1..-9: c-1
		draw_symbol_on_matrix(str_symbols[1], 4, 0);
		draw_symbol_on_matrix(str_symbols[2], 8, 0);
	} else if (str_symbols[0] == '>' || str_symbols[0] == '<') { // in moving up/down: >10 or >1c
		draw_symbol_on_matrix(str_symbols[0], 1, 0);
		draw_symbol_on_matrix(str_symbols[1], 7, 0);
		draw_symbol_on_matrix(str_symbols[2], 11, 0);
	}

	// for str without symbols 'c' and '<', '>', for example, "OK"
	else if (strlen(str_symbols) == 3) { // 3 symbols, font_width = 4
		draw_symbol_on_matrix(str_symbols[0], 1, 0);
		draw_symbol_on_matrix(str_symbols[1], 6, 0);
		draw_symbol_on_matrix(str_symbols[2], 11, 0);
	} else if (strlen(str_symbols) == 2) { // 2 symbols, font_width = 4
		draw_symbol_on_matrix(str_symbols[0], 2, 0);
		draw_symbol_on_matrix(str_symbols[1], 10, 0);
	} else if (strlen(str_symbols) == 1) { // 1 symbol, font_width = 4
		draw_symbol_on_matrix(str_symbols[0], 6, 0);
	}
}
