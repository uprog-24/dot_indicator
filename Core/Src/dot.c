#include "stm32f1xx_hal.h"
#include "dot.h"
#include "tim.h"

#define ROWS 8
#define COLUMNS 16

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} PinConfig;

PinConfig rows[] = { { ROW_1_GPIO_Port, ROW_1_Pin }, { ROW_2_GPIO_Port,
ROW_2_Pin }, { ROW_3_GPIO_Port, ROW_3_Pin }, { ROW_4_GPIO_Port,
ROW_4_Pin }, { ROW_5_GPIO_Port, ROW_5_Pin }, { ROW_6_GPIO_Port,
ROW_6_Pin }, { ROW_7_GPIO_Port, ROW_7_Pin }, { ROW_8_GPIO_Port,
ROW_8_Pin }, };

PinConfig cols[] = {

{ COL_R1_GPIO_Port, COL_R1_Pin }, { COL_R2_GPIO_Port, COL_R2_Pin }, {
COL_R3_GPIO_Port, COL_R3_Pin }, { COL_R4_GPIO_Port, COL_R4_Pin }, {
COL_R5_GPIO_Port, COL_R5_Pin }, { COL_R6_GPIO_Port, COL_R6_Pin }, {
COL_R7_GPIO_Port, COL_R7_Pin }, { COL_R8_GPIO_Port, COL_R8_Pin },

{ COL_L1_GPIO_Port, COL_L1_Pin }, { COL_L2_GPIO_Port, COL_L2_Pin }, {
COL_L3_GPIO_Port, COL_L3_Pin }, { COL_L4_GPIO_Port, COL_L4_Pin }, {
COL_L5_GPIO_Port, COL_L5_Pin }, { COL_L6_GPIO_Port, COL_L6_Pin }, {
COL_L7_GPIO_Port, COL_L7_Pin }, { COL_L8_GPIO_Port, COL_L8_Pin },

};

int matrix[ROWS][COLUMNS] = { 0 };
uint8_t current_col = 0;
uint8_t current_row = 0;

void set_row_state(uint8_t row, states_t state) {
	switch (state) {

	case TURN_OFF:
		HAL_GPIO_WritePin(rows[row].port, rows[row].pin, GPIO_PIN_RESET);
		break;

	case TURN_ON:
		HAL_GPIO_WritePin(rows[row].port, rows[row].pin, GPIO_PIN_SET);
		break;

	}
}

void set_col_state(uint8_t col, states_t state) {
	switch (state) {

	case TURN_OFF:
		HAL_GPIO_WritePin(cols[col].port, cols[col].pin, GPIO_PIN_RESET);
		break;

	case TURN_ON:
		HAL_GPIO_WritePin(cols[col].port, cols[col].pin, GPIO_PIN_SET);
		break;

	}
}

void set_all_cols_state(states_t state) {
	for (int col = 0; col < COLUMNS; col++) {
		set_col_state(col, state);
	}
}

void set_all_rows_state(states_t state) {
	for (uint8_t row = 0; row < ROWS; row++) {
		set_row_state(row, state);
	}
}

void set_full_matrix_state(states_t state) {
	set_all_rows_state(state);
	set_all_cols_state(state);
}

void set_matrix_by_rows() {

	while (current_row < ROWS) {

		set_all_cols_state(TURN_OFF);

		set_row_state(current_row, TURN_ON);
		while (current_col < COLUMNS) {
			set_col_state(current_col, TURN_ON);
			TIM3_Delay_ms(100);
			current_col++;
		}

		for (uint8_t r = 0; r < current_row; r++) {
			set_row_state(r, TURN_ON);
		}

		current_row++;
		current_col = 0;
	}

}

