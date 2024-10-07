/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include <stdbool.h>
#include <string.h>
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
uint8_t uart1_rx_cnt = 0; // Receive buffer count
char rx_byte = ' ';

volatile bool is_data_ready_to_be_read = false;
volatile bool is_rx_full_completed = false;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		is_data_ready_to_be_read = true;
		is_rx_full_completed = true;
	}
}

volatile bool is_i2c_master_tx_completed = false;
volatile bool is_tx_full_completed = false;
/* Регистр TXE пуст */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		is_i2c_master_tx_completed = true;
		is_tx_full_completed = true;
	}
}
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void receive_data_from_uart(char *rx_buff) {
//	char rx_byte = ' ';
//	char cnt;
//	uint8_t isDataEnd = 0;
//
//	memset(rx_buff, 0, strlen(rx_buff));
//	uart1_rx_cnt = 0;
//	while (!isDataEnd) {
//		HAL_UART_Receive_IT(&huart1, (uint8_t*) &rx_byte, 1);
//
//		if (is_rx_full_completed) {
//			is_rx_full_completed = false;
//
//			if (rx_byte != '\0') { // chars end: NUL
//				rx_buff[uart1_rx_cnt++] = rx_byte; // receive byte
//			} else {
//				cnt = uart1_rx_cnt + '0';
//				isDataEnd = 1;
//			}
//		}
//	}

//	char rx_byte = ' ';
	char cnt;
	uint8_t isDataEnd = 0;

	memset(rx_buff, 0, strlen(rx_buff));
	uart1_rx_cnt = 0;
	while (!isDataEnd) {
		HAL_UART_Receive_IT(&huart1, (uint8_t*) &(rx_byte), 1);

		if (is_data_ready_to_be_read) {
			is_data_ready_to_be_read = false;

			rx_buff[uart1_rx_cnt++] = rx_byte; // Receive byte

			if ((rx_buff[uart1_rx_cnt - 1] == 0x0A)
					&& (rx_buff[uart1_rx_cnt - 2] == 0x0D)) { // Chars end: CR+LF
				cnt = uart1_rx_cnt + '0';
				isDataEnd = 1;
			}
		}

	}
}

/* USER CODE END 1 */
