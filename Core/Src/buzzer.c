/**
 * @file buzzer.c
 */
#include "buzzer.h"
#include "tim.h"

/**
 * @brief  Setting the state of an active buzzer.
 * When state = TURN_ON - buzzer is turning on,
 * when state = TURN_OFF - buzzer is turning off.
 * @param  state Type states_t: TURN_ON, TURN_OFF.
 * @retval None
 */
void set_active_buzzer_state(states_t state) {
	switch (state) {

	case TURN_OFF:
		HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);
		break;

	case TURN_ON:
		HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_SET);
		break;

	}
}

/**
 * @brief  Setting the melody of an passive buzzer.
 * @param  freq_buff Buffer with frequencies for buzzer's melody. Last element: 0 Hz - to stop the sound.
 * @param buff_size Size of freq_buff.
 * @retval None
 */
void set_passive_buzzer_melody(uint16_t *freq_buff, uint8_t buff_size) {
	for (uint8_t ind_freq = 0; ind_freq < buff_size - 1; ind_freq++) {
		TIM2_PWM_Frequency(freq_buff[ind_freq]);
		TIM3_Delay_ms(250);
	}

	TIM2_PWM_Frequency(freq_buff[buff_size - 1]);
}
