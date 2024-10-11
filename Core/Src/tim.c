/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "font.h"

#define TIM2_FREQ 64000000 ///< Frequency of APB1 for TIM2.
#define TIM2_PERIOD 1000 ///< Period of TIM2.
#define TIM3_FREQ TIM2_FREQ ///< Frequency of APB1 for TIM3.

#define TIM4_FREQ TIM2_FREQ ///< Frequency of APB1 for TIM4.
#define FREQ_FOR_S 1 ///< Frequency of TIM4 for Delay in sec.

#define FREQ_FOR_MS 1000 ///< Frequency of TIM3 for Delay in ms.
#define FREQ_FOR_US 1000000 ///< Frequency of TIM3 for Delay in us.

//#define PRESCALER_FOR_SEC TIM4_FREQ/FREQ_FOR_S -1  ///< Prescaler for TIM4 for Delay in sec. // 64000-1

#define PRESCALER_FOR_MS TIM2_FREQ/FREQ_FOR_MS -1  ///< Prescaler for TIM3 for Delay in ms. // 64000-1
#define PRESCALER_FOR_US TIM2_FREQ/FREQ_FOR_US - 1 ///< Prescaler for TIM3 for Delay in us. // 64-1

/// Flag to control if period of TIM3 is elapsed
volatile bool is_tim3_period_elapsed = false;

/// Flag to control if period of TIM4 is elapsed
volatile bool is_tim4_period_elapsed = false;

volatile uint32_t tim4_elapsed_ms = 0;         // Счетчик прошедшего времени
volatile bool action_625_triggered = false;    // Флаг для 625 мс
volatile bool action_1250_triggered = false;   // Флаг для 1250 мс

/**
 * @brief  Handle Interrupt by period of TIM3 is elapsed,
 * setting the state of the flag.
 * @param  *htim: Structure of TIM
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM3) {
		is_tim3_period_elapsed = true;
	}

	if (htim->Instance == TIM4) {
		is_tim4_period_elapsed = true;
	}

}
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* TIM2 init function */
void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 500;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}
/* TIM3 init function */
void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 64000 - 1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 100 - 1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}
/* TIM4 init function */
void MX_TIM4_Init(void) {

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 64000 - 1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 100 - 1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {

	if (tim_baseHandle->Instance == TIM2) {
		/* USER CODE BEGIN TIM2_MspInit 0 */

		/* USER CODE END TIM2_MspInit 0 */
		/* TIM2 clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();

		/* TIM2 interrupt Init */
		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		/* USER CODE BEGIN TIM2_MspInit 1 */

		/* USER CODE END TIM2_MspInit 1 */
	} else if (tim_baseHandle->Instance == TIM3) {
		/* USER CODE BEGIN TIM3_MspInit 0 */

		/* USER CODE END TIM3_MspInit 0 */
		/* TIM3 clock enable */
		__HAL_RCC_TIM3_CLK_ENABLE();

		/* TIM3 interrupt Init */
		HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		/* USER CODE BEGIN TIM3_MspInit 1 */

		/* USER CODE END TIM3_MspInit 1 */
	} else if (tim_baseHandle->Instance == TIM4) {
		/* USER CODE BEGIN TIM4_MspInit 0 */

		/* USER CODE END TIM4_MspInit 0 */
		/* TIM4 clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();

		/* TIM4 interrupt Init */
		HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
		/* USER CODE BEGIN TIM4_MspInit 1 */

		/* USER CODE END TIM4_MspInit 1 */
	}
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (timHandle->Instance == TIM2) {
		/* USER CODE BEGIN TIM2_MspPostInit 0 */

		/* USER CODE END TIM2_MspPostInit 0 */

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM2 GPIO Configuration
		 PA1     ------> TIM2_CH2
		 */
		GPIO_InitStruct.Pin = BUZZ_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(BUZZ_GPIO_Port, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM2_MspPostInit 1 */

		/* USER CODE END TIM2_MspPostInit 1 */
	}

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle) {

	if (tim_baseHandle->Instance == TIM2) {
		/* USER CODE BEGIN TIM2_MspDeInit 0 */

		/* USER CODE END TIM2_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();

		/* TIM2 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
		/* USER CODE BEGIN TIM2_MspDeInit 1 */

		/* USER CODE END TIM2_MspDeInit 1 */
	} else if (tim_baseHandle->Instance == TIM3) {
		/* USER CODE BEGIN TIM3_MspDeInit 0 */

		/* USER CODE END TIM3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM3_CLK_DISABLE();

		/* TIM3 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		/* USER CODE BEGIN TIM3_MspDeInit 1 */

		/* USER CODE END TIM3_MspDeInit 1 */
	} else if (tim_baseHandle->Instance == TIM4) {
		/* USER CODE BEGIN TIM4_MspDeInit 0 */

		/* USER CODE END TIM4_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM4_CLK_DISABLE();

		/* TIM4 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM4_IRQn);
		/* USER CODE BEGIN TIM4_MspDeInit 1 */

		/* USER CODE END TIM4_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */

/**
 * @brief  Set delay in milliseconds.
 * @param  delay Number between 1..65535
 * @retval None
 */
void TIM3_Delay_ms(uint16_t delay) {
	is_tim3_period_elapsed = false;

	__HAL_TIM_SET_PRESCALER(&htim3, PRESCALER_FOR_MS);
	__HAL_TIM_SET_AUTORELOAD(&htim3, delay);
	HAL_TIM_Base_Start_IT(&htim3);
	while (!is_tim3_period_elapsed) {
	}
	HAL_TIM_Base_Stop_IT(&htim3);
}

/**
 * @brief  Set delay in microseconds.
 * @param  delay Number between 1..65535
 * @retval None
 */
void TIM3_Delay_us(uint16_t delay) {
	is_tim3_period_elapsed = false;

	__HAL_TIM_SET_PRESCALER(&htim3, PRESCALER_FOR_US);
	__HAL_TIM_SET_AUTORELOAD(&htim3, delay);
	HAL_TIM_Base_Start_IT(&htim3);
	while (!is_tim3_period_elapsed) {
	}
	HAL_TIM_Base_Stop_IT(&htim3);
}

/**
 * @brief  Get prescaler for TIM2 (PWM) by current frequency.
 * prescaler = tim_freq / (tim_period_ARR * buzz_signal_freq)
 * @param  frequency Number between 1..65535
 * @retval prescaler
 */
int16_t TIM2_get_prescaler_frequency(int16_t frequency) {
	if (frequency == 0)
		return 0;
	return ((TIM2_FREQ / (TIM2_PERIOD * frequency)) - 1);
}

/**
 * @brief  Set frequency for sound of buzzer (turning on).
 * @param  frequency Number between 1..65535
 * @retval None
 */
void TIM2_PWM_Frequency(int16_t frequency) {
	__HAL_TIM_SET_PRESCALER(&htim2, TIM2_get_prescaler_frequency(frequency));
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);
}

uint32_t counter_value = 0;
uint16_t ms_counter = 0;

void TIM4_Set_symbol(uint16_t time_ms, char *str_symbols) {
	is_tim4_period_elapsed = false;

	tim4_elapsed_ms = 0;
	action_625_triggered = false;
	action_1250_triggered = false;

	ms_counter = 0;

	__HAL_TIM_SET_PRESCALER(&htim4, PRESCALER_FOR_MS);
	__HAL_TIM_SET_AUTORELOAD(&htim4, 1000);

	while (ms_counter < time_ms) {
		HAL_TIM_Base_Start_IT(&htim4);
		is_tim4_period_elapsed = false;
		while (!is_tim4_period_elapsed) {

			if (str_symbols[0] == 'c' && str_symbols[1] == 'c') {
				set_symbol(str_symbols[2], 5, 0);
			} else if (str_symbols[0] == 'c') {
				set_symbol(str_symbols[1], 4, 0);
				set_symbol(str_symbols[2], 8, 0);
			} else {
				set_symbol(str_symbols[0], 1, 0);
				set_symbol(str_symbols[1], 7, 0);

				switch (ms_counter) {
				case 0:
					set_symbol(str_symbols[2], 11, 0);
					break;
				case 1000:
					set_symbol(str_symbols[2], 11, 1);
					break;

				case 2000:
					set_symbol(str_symbols[2], 11, 2);
					break;
				case 3000:
					set_symbol(str_symbols[2], 11, 3);
					break;

				case 4000:
					set_symbol(str_symbols[2], 11, 4);
					break;
				case 5000:
					set_symbol(str_symbols[2], 11, 5);
					break;

				case 6000:
					set_symbol(str_symbols[2], 11, 6);
					break;
				case 7000:
					set_symbol(str_symbols[2], 11, 7);
					break;
				case 8000:
					set_symbol(str_symbols[2], 11, 8);
					break;
				}

			}

		}
		HAL_TIM_Base_Stop_IT(&htim4);
		ms_counter += 1000;
	}
//	HAL_TIM_Base_Stop_IT(&htim4);
}

/* USER CODE END 1 */
