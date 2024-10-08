/**
 * @file    buzzer.h
 * @brief   This file contains all the function prototypes for
 *          the buzzer.c file
 */
#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "main.h"

#include <stdint.h>

/** Setting the state of an active buzzer.
 * When state = TURN_ON - buzzer is turning on,
 * when state = TURN_OFF - buzzer is turning off.
 * @param[in] state Type states_t: TURN_ON, TURN_OFF.
 */
void set_active_buzzer_state(states_t state);

/** Setting the melody of an passive buzzer.
 * @param[in] freq_buff Buffer with frequencies for buzzer's melody. Last element: 0 Hz - to stop the sound.
 * @param[in] buff_size Size of freq_buff.
 */
void set_passive_buzzer_melody(uint16_t *freq_buff, uint8_t buff_size);

#endif /*__ BUTTON_H__ */
