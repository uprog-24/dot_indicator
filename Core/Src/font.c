#include "font.h"
#include "tim.h"
#include "dot.h"

#define FONT_WIDTH 4U
#define FONT_HEIGHT 8U
#define MAX_CHARS_ON_SCREEN 2U
#define SCREEN_WIDTH (MAX_CHARS_ON_SCREEN * FONT_WIDTH)
#define SCREEN_HEIGHT FONT_HEIGHT
#define EMPTY_SYMBOL_IDX 10U

uint8_t symbols[][8] = {
		{ // 0
				0b00000000,
				0b01110000,
				0b01010000,
				0b01010000,
				0b01010000,
				0b01010000,
				0b01110000,
				0b00000000
		},

		{ // 1
				0b00000000,
				0b00100000,
				0b01100000,
				0b00100000,
				0b00100000,
				0b00100000,
				0b01110000,
				0b00000000
		},
		{ // 2
				0b00000000,
				0b00110000,
				0b01010000,
				0b00010000,
				0b00100000,
				0b01000000,
				0b01110000,
				0b00000000
		},

		{
		// O
				0b00000000,
				0b00110000,
				0b01001000,
				0b01001000,
				0b01001000,
				0b01001000,
				0b00110000,
				0b00000000

		}, { // K
						0b00000000,
						0b01001000,
						0b01010000,
						0b01100000,
						0b01100000,
						0b01010000,
						0b01001000,
						0b00000000 } };

int8_t output[8];

char screen_array[SCREEN_HEIGHT][SCREEN_WIDTH];
int8_t digits[MAX_CHARS_ON_SCREEN] = { EMPTY_SYMBOL_IDX, };
int8_t input_number = 0;
int8_t number_of_digits = 0;

void clear_digits_mas() {
	for (int i = 0; i < MAX_CHARS_ON_SCREEN; i++) {
		digits[i] = EMPTY_SYMBOL_IDX;
	}
}

int get_number_len(int *num) {
	if (*num > 9 && *num < 100) {
		return 2;
	} else if (*num < 10 && *num > -1) {
		return 1;
	} else
		return -1;
}

void split_number_by_digits(int number, int number_of_digits) {

	clear_digits_mas();

	switch (number_of_digits) {
	case 2:
		digits[1] = number / 10;
		break;
	case 3:
		digits[0] = number / 100;
		digits[1] = (number - 100 * digits[0]) / 10;
		break;
	}
	digits[2] = number % 10;
}

void from_10_to_2(int input, int *output) {
	int i = 7;
	while (i != -1) {
		output[i] = input % 2;
		input /= 2;
		i--;
	}
}

void set_digit(int d, int pos, char *mas, int rows, int cols) {
	uint8_t *cur_item = 0;

	cur_item = symbols[d];

	for (uint8_t cur_row = 0; cur_row < 8; cur_row++) {
		for (int row = 0; row < rows; row++) {
			if (row == cur_row) {
				set_row_state(cur_row, TURN_ON);
			} else {
				set_row_state(row, TURN_OFF);
			}
		}

		int output[8];
		int num_bit = 0;

		from_10_to_2(cur_item[cur_row], output);

		int start_col = pos; //* 8;
		int end_col = start_col + 5;
		for (int col = start_col; col < end_col; col++) {
			if (output[num_bit] == 1) {
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

void init_screen() {
	memset((void*) screen_array, '-',
	SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(char));
}

void set_symbols_to_screen() {
//	init_screen();

//	for (int digit = 0; digit < MAX_CHARS_ON_SCREEN; digit++) {
//		set_digit(digits[digit], digit, &(screen_array[0][0]),
//		SCREEN_HEIGHT, SCREEN_WIDTH);
//	}

	set_digit(3, 1, &(screen_array[0][0]), 8, 8);
	set_digit(4, 9, &(screen_array[0][0]), 8, 8);
}

void print_number_to_screen(int *number) {
	split_number_by_digits(*number, get_number_len(number));
	set_digits_to_screen();

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int k = 0; k < SCREEN_WIDTH; k++) {
			printf("%c", screen_array[i][k]);
		}
		printf("%c", '\n');
	}
}
