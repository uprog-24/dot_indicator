/**
 * @file demo_mode.c
 */
#include <uim6100.h>
#include "buzzer.h"
#include "drawing.h"
#include "tim.h"

#include <stdbool.h>

#define ARROW_MASK 0x03 ///< Mask for indicator UP/DOWN (0 and 1st bits of byte W3).
#define ARRIVAL_MASK 0x04 ///< Mask for bit arrival (2nd bit of byte W3).

#define UPWARD_VALUE 2 ///< Bit value of indicator UP (bits: 10).
#define DOWNWARD_VALUE 1 ///< Bit value of indicator DOWN (bits: 01).
#define ARRIVAL_VALUE 4 ///< Bit value of arrival (bits: 100).

#define BUZZER_FREQ 3000 ///< Frequency of bip for start UPWARD, DOWNWARD and ARRIVAL.
#define BUZZER_FREQ_CABIN_OVERLOAD 5000 ///< Frequency of bip for VOICE_CABIN_OVERLOAD.

/** Stores previous and current state of bit to control buzzer. */
static uint8_t gong[2] = { 0, };

code_floor_symbol_t code_floor_symbols[] = { { RESERVE, 'b' }, { SEISMIC_DANGER,
		'L' }, { LIFT_NOT_WORK, 'A' }, { TRANSFER_FIREFIGHTERS, 'P' }, {
		CODE_FLOOR_54, 'H' }, { SERVICE, 'C' }, { EVACUATION, 'E' }, {
		FIRE_DANGER, 'F' }, { FAULT_IBP, 'U' }, { LOADING, 'p' } };

/**
 * @brief  Get value of direction from byte W3.
 * @param  direction_byte_w_3: Byte W3.
 * @retval Value of direction: 0 (bits: 00) - STOP,
 * 1 (bits: 01) - DOWNWARD, 2 (bits: 10) - UPWARD
 */
uint8_t get_direction(uint8_t direction_byte_w_3) {
	return direction_byte_w_3 & ARROW_MASK;
}

volatile bool is_cabin_overload = false;
void process_code_msg(uint8_t code_msg_byte_w_1) {
//	if (code_msg_byte_w_1 == VOICE_CABIN_OVERLOAD) {
//		TIM2_Start_bip(BUZZER_FREQ_CABIN_OVERLOAD);
//	} else {
//		TIM2_Stop_bip();
//	}
	static uint32_t last_bip_time = 0;

	if (code_msg_byte_w_1 == VOICE_CABIN_OVERLOAD) {
		is_cabin_overload = true;
		uint32_t current_time = HAL_GetTick();

//	        if (current_time - last_bip_time >= 200) {
//	        TIM2_Stop_bip();
		TIM2_Start_bip(BUZZER_FREQ_CABIN_OVERLOAD);
//	            last_bip_time = current_time;
//	        }
	} else

	if (is_cabin_overload) {
		TIM2_Stop_bip();
		is_cabin_overload = false;
	}
}

/**
 * @brief  Setting gong depend on direction bits in byte W3.
 * @param  direction_byte_w_3: Byte W3.
 * @retval None
 */
void setting_gong(uint8_t direction_byte_w_3) {
	direction_t direction = direction_byte_w_3 & ARROW_MASK;

	uint8_t arrival = direction_byte_w_3 & ARRIVAL_MASK;

//	switch (direction) {
//	case UPWARD:
//		gong[0] = (direction == UPWARD_VALUE) != 0 ? 1 : 0;
//		break;
//	case DOWNWARD:
//		gong[0] = (direction == DOWNWARD_VALUE) != 0 ? 1 : 0;
//		break;
//	case STOP:
	gong[0] = (arrival == ARRIVAL_VALUE) != 0 ? 1 : 0;
//		break;
//	}

	if (gong[0] && !gong[1]) {

		switch (direction) {
		case UPWARD:
			play_gong(1, BUZZER_FREQ);
			break;
		case DOWNWARD:
			play_gong(2, BUZZER_FREQ);
			break;
		case STOP:
			play_gong(3, BUZZER_FREQ);
			break;
		}
	}
	gong[1] = gong[0];
}