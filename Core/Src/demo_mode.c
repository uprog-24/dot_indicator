/**
 * @file demo_mode.c
 */
#include "demo_mode.h"
#include "dot.h"
#include "font.h"
#include "tim.h"
#include "drawing.h"

#include <stdlib.h>
#include <string.h>

#define STOP_FLOORS_BUFF_SIZE 4 ///< Size of buff_stop_floors.

static uint8_t buff_stop_floors[STOP_FLOORS_BUFF_SIZE] = { 7, 8, 10, 11 }; ///< Buffer with stop floors.

#define DISPLAY_STR_DURING_MS 2000 ///< Time during which symbols are displayed on matrix.

/**
 * @brief  Movement from start to finish floor with stop floors.
 * @param  start_floor Start floor.
 * @param  finish_floor Finish floor.
 * @param  *buff_stop_floors Pointer to the buffer with stop floors.
 * @param  buff_stop_size Size of the buff_stop_floors.
 * @retval None
 */
static void demo_start_finish_floors_movement(uint8_t start_floor,
		uint8_t finish_floor, uint8_t *buff_stop_floors, uint8_t buff_stop_size) {

	char temp_str[3];
	uint8_t current_floor = start_floor;

	// start floor
	setting_symbols_floor(temp_str, start_floor, STOP);
	TIM4_Diaplay_symbols_on_matrix(DISPLAY_STR_DURING_MS, temp_str);

	// other floors
	while (abs(current_floor - finish_floor) > 0) {

		if (buff_stop_size != 0 && buff_stop_floors != NULL) {
			for (uint8_t ind = 0; ind < buff_stop_size; ind++) {
				if (current_floor == buff_stop_floors[ind]) {
					setting_symbols_floor(temp_str, current_floor, STOP);
					TIM4_Diaplay_symbols_on_matrix(DISPLAY_STR_DURING_MS,
							temp_str);
					break;
				}
			}
		}

		if (finish_floor > start_floor) {
			setting_symbols_floor(temp_str, current_floor, UPWARD);
			current_floor++;
		} else {
			setting_symbols_floor(temp_str, current_floor, DOWNWARD);
			current_floor--;
		}

		TIM4_Diaplay_symbols_on_matrix(DISPLAY_STR_DURING_MS, temp_str);
	}

	// finish floor
	setting_symbols_floor(temp_str, finish_floor, STOP);
	TIM4_Diaplay_symbols_on_matrix(DISPLAY_STR_DURING_MS, temp_str);

}

/**
 * @brief  Start lift movement in demo mode.
 * @retval None
 */
void demo_mode_start(void) {
	demo_start_finish_floors_movement(1, 14, buff_stop_floors,
	STOP_FLOORS_BUFF_SIZE);
	demo_start_finish_floors_movement(14, 1, NULL, 0);
}
