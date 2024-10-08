/**
 * @file button.c
 */
#include "main.h"
#include "button.h"
#include "buzzer.h"
#include "dot.h"
#include "tim.h"

#include <stdbool.h>

#define BUZZER_FREQ 3000 ///< Frequency for buzzer's sound.

/// Flag to control BUTTON_1 state
volatile bool is_button_1_pressed = false;

/// Flag to control BUTTON_2 state
volatile bool is_button_2_pressed = false;

/**
  * @brief  Handle Interrupt by EXTI line, setting the state when button is pressed.
  * @param  GPIO_Pin: Button's pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == BUTTON_1_Pin) {
		is_button_1_pressed = true;
	}

	if (GPIO_Pin == BUTTON_2_Pin) {
		is_button_2_pressed = true;
	}

}

/// Counter for pressing of BUTTON_1
static uint8_t btn_1_counter = 1;

/// Counter for pressing of BUTTON_2
static uint8_t btn_2_counter = 1;

/**
 * @brief  Handle pressing BUTTON_1 and BUTTON_2.
 * When BUTTON_1 is pressed 1st time - matrix is turning on,
 * pressed 2nd time - matrix is turning off.
 * When BUTTON_2 is pressed 1st time - buzzer is turning on,
 * pressed 2nd time - buzzer is turning off.
 * @param  None
 * @retval None
 */
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
			TIM2_PWM_Frequency(BUZZER_FREQ);
			break;
		case 2:
			TIM2_PWM_Frequency(0);
			break;
		}

		btn_2_counter = (btn_2_counter < 2) ? btn_2_counter + 1 : 1;

	}
}
