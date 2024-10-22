/**
 * @file    shk6100.h
 * @brief   This file contains all the function prototypes for
 *          the shk6100.c file
 */
#ifndef UIM100_H
#define UIM6100_H

#include <stdint.h>

#define UIM6100_DLC 6 ///< Length of data (6 bit).
#define UIM6100_MAIN_CABIN_CAN_ID 46 ///< ID of the main cabin
#define BYTE_CODE_OPERATION_0_VALUE 0x81 ///< Value of BYTE_CODE_OPERATION_0 byte
#define BYTE_CODE_OPERATION_1_VALUE 0x00 ///< Value of BYTE_CODE_OPERATION_1 byte

/**
 * Stores indexes of bytes of UIM6100 protocol.
 */
enum uim6100_packet_bytes {
	BYTE_CODE_OPERATION_0 = 0,
	BYTE_CODE_OPERATION_1,
	BYTE_W_0,
	BYTE_W_1,
	BYTE_W_2,
	BYTE_W_3
};

typedef enum CODE_MSG {
	VOICE_LIFT_IS_NOT_WORK = 55,
	VOICE_FIRE_DANGER = 56,
	VOICE_CABIN_OVERLOAD = 57,
	VOICE_DOORS_CLOSING = 58,
	VOICE_DOORS_OPENING = 59,
	SOUND_DOORS_CLOSING = 60,
	SOUND_DOORS_OPENING = 61,
	BUTTON_SOUND_SHORT = 62,
	MUTE_SOUND = 63
} code_msg_t;

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

#define CODE_FLOOR_SYMBOLS_NUMBER 10
extern code_floor_symbol_t code_floor_symbols[];
/**
 * @brief  Get value of direction from byte W3.
 * @param  direction_byte_w_3: Byte W3.
 * @retval Value of direction: 0 (bits: 00) - STOP,
 * 1 (bits: 01) - DOWNWARD, 2 (bits: 10) - UPWARD
 */
uint8_t get_direction(uint8_t direction_byte_w_3);

void process_code_msg(uint8_t code_msg_byte_w_1);

/**
 * @brief  Setting gong depend on direction bits in byte W3.
 * @param  direction_byte_w_3: Byte W3.
 * @retval None
 */
void setting_gong(uint8_t direction_byte_w_3);

#endif // UIM6100_H
