/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.h
 * @brief   This file contains all the function prototypes for
 *          the tim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */

/**
 * @brief  Set delay in milliseconds.
 * @param  delay Number between 1..65535
 * @retval None
 */
void TIM3_Delay_ms(uint16_t delay);

/**
 * @brief  Set delay in microseconds.
 * @param  delay Number between 1..65535
 * @retval None
 */
void TIM3_Delay_us(uint16_t delay);

/**
 * @brief  Set frequency for sound of buzzer (turning on).
 * @param  frequency: Number between 1..65535
 * @retval None
 */
void TIM2_Start_bip(uint16_t frequency);

/**
 * @brief  Turn off the sound of buzzer.
 * Stop bip using prescaler of TIM2.
 * @retval None
 */
void TIM2_Stop_bip();

/**
 * @brief  Display symbols on matrix.
 * @param  time_ms The time (ms) during which the symbols will be displayed.
 * @param  *str_symbols Pointer to the string to be displayed.
 * @retval None
 */
void TIM4_Diaplay_symbols_on_matrix(uint16_t time_ms, char *str_symbols);

/**
 * @brief  Start TIM1 using CH1 Output Compare mode
 * to control bip duration in ms.
 * @retval None
 */
void TIM1_Start();

/**
 * @brief  Start TIM4 to control connection of CAN,
 * if through 3 sec new data wasn't received, CAN isn't connected.
 * @retval None
 */
void TIM4_Start();

/**
 * @brief  Start TIM2 for buzzer.
 * @retval None
 */
void TIM2_Start_PWM();

/**
 * @brief  Set frequency, bip_counter, bip_duration_ms, and start bip sequence.
 * @param  frequency: Frequency for buzzer sound.
 * @param  bip_counter: Number of bips.
 * @param  bip_duration_ms Duration of one bip.
 * @retval None
 */
void TIM2_Set_pwm_sound(uint16_t frequency, uint16_t bip_counter,
		uint8_t bip_duration_ms);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

