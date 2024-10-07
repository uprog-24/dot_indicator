#include "buzzer.h"
#include "tim.h"

#define BUZZER_FREQ 3000

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

void set_passive_buzzer_melody(uint16_t *freq_buff, uint8_t buff_size) {
	for (uint8_t ind_freq = 0; ind_freq < buff_size - 1; ind_freq++) {
		TIM2_PWM_Frequency(freq_buff[ind_freq]);
		TIM3_Delay_ms(250);
	}

	TIM2_PWM_Frequency(freq_buff[buff_size - 1]);
}
