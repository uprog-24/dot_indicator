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
#include "drawing.h"

#define TIM2_FREQ 64000000 ///< Frequency of APB1 for TIM2.
#define TIM2_PERIOD 1200 ///< Period of TIM2.
#define TIM3_FREQ TIM2_FREQ ///< Frequency of APB1 for TIM3.

#define TIM4_PERIOD TIM2_PERIOD ///< Period of TIM4.

#define TIM4_FREQ TIM2_FREQ ///< Frequency of APB1 for TIM4.
#define FREQ_FOR_S 1 ///< Frequency of TIM4 for Delay in sec.

#define FREQ_FOR_MS 1000 ///< Frequency of TIM3 for Delay in ms.
#define FREQ_FOR_US 1000000 ///< Frequency of TIM3 for Delay in us.

#define PRESCALER_FOR_MS TIM2_FREQ/FREQ_FOR_MS -1  ///< Prescaler for TIM3 for Delay in ms. // 64000-1
#define PRESCALER_FOR_US TIM2_FREQ/FREQ_FOR_US - 1 ///< Prescaler for TIM3 for Delay in us. // 64-1

#define DISPLAY_STR_DURING_MS 2000

/// Flag to control if period of TIM2 is elapsed
volatile bool is_tim2_period_elapsed = false;

/// Flag to control if period of TIM3 is elapsed
volatile bool is_tim3_period_elapsed = false;

/// Flag to control if period of TIM4 is elapsed
volatile bool is_tim4_period_elapsed = false;

/// TIM4 counter to control elapsed time in ms
volatile uint32_t tim4_elapsed_ms = 0;

/// TIM1 counter to control elapsed time in ms
volatile uint32_t tim1_elapsed_ms = 0;

extern volatile uint32_t alive_cnt[2];
extern bool is_can_connected;

//volatile bool is_buzzer_sound = false;
volatile uint16_t _bip_freq = 0;
volatile uint8_t _bip_counter = 0;
volatile uint8_t _bip_duration_ms = 0;

/**
 * @brief  Get prescaler for TIM2 (PWM) by current frequency.
 * prescaler = tim_freq / (tim_period_ARR * buzz_signal_freq)
 * @param  frequency Number between 1..65535
 * @retval prescaler
 */
uint16_t TIM2_get_prescaler_frequency(uint16_t frequency) {
	if (frequency == 0)
		return 0;
	return ((TIM2_FREQ / (TIM2_PERIOD * frequency)) - 1);
}

/**
 * @brief  Set frequency for sound of buzzer (turning on).
 * @param  frequency Number between 1..65535
 * @retval None
 */
void TIM2_Start_bip(uint16_t frequency) {
	uint16_t prescaler = TIM2_get_prescaler_frequency(frequency);
	__HAL_TIM_SET_PRESCALER(&htim2, prescaler);
}

/**
 * @brief  Turn off the sound of buzzer.
 * Stop bip using prescaler of TIM2.
 * @retval None
 */
void TIM2_Stop_bip() {
	uint16_t prescaler = 0;
	__HAL_TIM_SET_PRESCALER(&htim2, prescaler);
}

/**
 * @brief  Stop TIM1, stop  count duration for bips.
 * @retval None
 */
void TIM1_Stop() {
	tim1_elapsed_ms = 0;
	HAL_TIM_Base_Stop_IT(&htim1);
	HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_1);
}

/**
 * @brief  Handle Interrupt by period of TIM3 is elapsed,
 * setting the state of the flag.
 * @param  *htim: Structure of TIM
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM2) {
		is_tim2_period_elapsed = true;
	}

	if (htim->Instance == TIM3) {
		is_tim3_period_elapsed = true;
	}

	if (htim->Instance == TIM4) {
		is_tim4_period_elapsed = true;

		if (alive_cnt[0] == alive_cnt[1]) {
			is_can_connected = false;
		}
		alive_cnt[1] = alive_cnt[0];

	}

}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {

		tim1_elapsed_ms++;

		if (tim1_elapsed_ms == _bip_duration_ms) { // stop bip 1

			TIM2_Stop_bip();

			if (_bip_counter == 1) {
				TIM1_Stop();
				TIM2_Stop_PWM();
			}

		}

		if (tim1_elapsed_ms == 2 * _bip_duration_ms) { // start bip 2
			TIM2_Start_bip(_bip_freq);

		}

		if (tim1_elapsed_ms == 3 * _bip_duration_ms) { // stop bip 2

			TIM2_Stop_bip();

			if (_bip_counter == 2) {
				TIM1_Stop();
				TIM2_Stop_PWM();
			}
		}

		if (tim1_elapsed_ms == 4 * _bip_duration_ms) { // start bip 3
			TIM2_Start_bip(_bip_freq);
		}

		if (tim1_elapsed_ms == 5 * _bip_duration_ms) { // stop bip 3

			TIM2_Stop_bip();

			if (_bip_counter == 3) {
				TIM1_Stop();
				TIM2_Stop_PWM();
			}

		}

	}
}
/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* TIM1 init function */
void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_OC_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}
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
	htim2.Init.Prescaler = 63;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
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
	sConfigOC.Pulse = 200;
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

	if (tim_baseHandle->Instance == TIM1) {
		/* USER CODE BEGIN TIM1_MspInit 0 */

		/* USER CODE END TIM1_MspInit 0 */
		/* TIM1 clock enable */
		__HAL_RCC_TIM1_CLK_ENABLE();

		/* TIM1 interrupt Init */
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
		/* USER CODE BEGIN TIM1_MspInit 1 */

		/* USER CODE END TIM1_MspInit 1 */
	} else if (tim_baseHandle->Instance == TIM2) {
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

	if (tim_baseHandle->Instance == TIM1) {
		/* USER CODE BEGIN TIM1_MspDeInit 0 */

		/* USER CODE END TIM1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM1_CLK_DISABLE();

		/* TIM1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
		/* USER CODE BEGIN TIM1_MspDeInit 1 */

		/* USER CODE END TIM1_MspDeInit 1 */
	} else if (tim_baseHandle->Instance == TIM2) {
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
 * @brief  Start TIM2 for buzzer.
 * @retval None
 */
void TIM2_Start_PWM() {
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);
}

/**
 * @brief  Stop TIM2 for buzzer.
 * @retval None
 */
void TIM2_Stop_PWM() {
	HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2);
}

/**
 * @brief  Set frequency, bip_counter, bip_duration_ms, and start bip sequence.
 * @param  frequency: Frequency for buzzer sound.
 * @param  bip_counter: Number of bips.
 * @param  bip_duration_ms Duration of one bip.
 * @retval None
 */
void TIM2_Set_pwm_sound(uint16_t frequency, uint16_t bip_counter,
		uint8_t bip_duration_ms) {

	_bip_freq = frequency;
	_bip_counter = bip_counter;
	_bip_duration_ms = bip_duration_ms;

	// start bip 1
	TIM2_Start_bip(frequency);

}

/**
 * @brief  Display symbols on matrix.
 * @param  time_ms The time (ms) during which the symbols will be displayed.
 * @param  *str_symbols Pointer to the string to be displayed.
 * @retval None
 */
void TIM4_Diaplay_symbols_on_matrix(uint16_t time_ms, char *str_symbols) {
	is_tim4_period_elapsed = false;

	uint16_t tim4_ms_counter = 0;

	__HAL_TIM_SET_PRESCALER(&htim4, PRESCALER_FOR_MS); // 1 ms
	__HAL_TIM_SET_AUTORELOAD(&htim4, TIM4_PERIOD); // 1 s

	while (tim4_ms_counter < time_ms) {
		HAL_TIM_Base_Start_IT(&htim4);
		is_tim4_period_elapsed = false;
		while (!is_tim4_period_elapsed) {
			draw_str_on_matrix(str_symbols);
		}
		HAL_TIM_Base_Stop_IT(&htim4);
		tim4_ms_counter += TIM4_PERIOD;
	}
}

/**
 * @brief  Start TIM1 CH1 Output Compare mode to control bip duration in ms.
 * @retval None
 */
void TIM1_Start() {
	__HAL_TIM_SET_PRESCALER(&htim1, PRESCALER_FOR_MS); // 1 ms
	__HAL_TIM_SET_AUTORELOAD(&htim1, 1); // 1 ms
	HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
}

/**
 * @brief  Start TIM4 to control connection of CAN,
 * if through 3 sec new data wasn't received, CAN isn't connected.
 * @retval None
 */
void TIM4_Start() {
	__HAL_TIM_SET_PRESCALER(&htim4, PRESCALER_FOR_MS); // 1 ms
	__HAL_TIM_SET_AUTORELOAD(&htim4, 3000); // 3 s
	HAL_TIM_Base_Start_IT(&htim4);
}

/* USER CODE END 1 */
