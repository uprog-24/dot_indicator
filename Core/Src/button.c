#include "main.h"
#include "button.h"
#include "buzzer.h"
#include "dot.h"

#define BUZZER_ON_BUFF_SIZE 3
#define BUZZER_OFF_BUFF_SIZE 1
#include <stdbool.h>

uint16_t buzzer_on_buff[BUZZER_ON_BUFF_SIZE] = { 3000, 4000, 0};
uint16_t buzzer_off_buff[BUZZER_OFF_BUFF_SIZE] = { 0};

volatile bool is_button_1_pressed = false;
volatile bool is_button_2_pressed = false;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == BUTTON_1_Pin) {
		is_button_1_pressed = true;
	}

	if (GPIO_Pin == BUTTON_2_Pin) {
		is_button_2_pressed = true;
	}

}

uint8_t btn_1_counter = 1;
uint8_t btn_2_counter = 1;
void press_button() {
	if (is_button_1_pressed) {
		is_button_1_pressed = false;

		switch (btn_1_counter) {
		case 1:
			set_full_matrix_state(TURN_ON);
			break;
		case 2:
			set_full_matrix_state(TURN_OFF);
			break;
		}

		btn_1_counter = (btn_1_counter < 2) ? btn_1_counter + 1 : 1;
	}

	if (is_button_2_pressed) {
		is_button_2_pressed = false;

		switch (btn_2_counter) {
		case 1:

			TIM2_PWM_Frequency(3000);
//			set_passive_buzzer_melody(buzzer_on_buff, BUZZER_ON_BUFF_SIZE);
			break;
		case 2:
//			set_passive_buzzer_state(TURN_OFF);
			TIM2_PWM_Frequency(0);
//			set_passive_buzzer_melody(buzzer_off_buff, BUZZER_OFF_BUFF_SIZE);
			break;
		}

		btn_2_counter = (btn_2_counter < 2) ? btn_2_counter + 1 : 1;

	}
}
