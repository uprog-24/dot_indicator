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

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

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
 * @param  frequency Number between 1..65535
 * @retval None
 */
void TIM2_PWM_Frequency(int16_t frequency);

/**
 * @brief  Display symbols on matrix.
 * @param  time_ms The time (ms) during which the symbols will be displayed.
 * @param  *str_symbols Pointer to the string to be displayed.
 * @retval None
 */
void TIM4_Diaplay_symbols_on_matrix(uint16_t time_ms, char *str_symbols);


//void demo_start_finish_floors_movement(uint8_t start_floor, uint8_t finish_floor,
//		uint8_t *buff_stop_floors, uint8_t buff_stop_size, char direction);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

