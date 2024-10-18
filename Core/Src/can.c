/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
#include "dot.h"
#include "font.h"
#include "drawing.h"
#include "tim.h"

#include <stdbool.h>
#include <stdio.h>

#define CAN_DATA_SIZE 8 ///< Size of data (8 bit).

enum uim6100_packet_bytes {
	BYTE_CODE_OPERATION_0 = 0,
	BYTE_CODE_OPERATION_1,
	BYTE_W_0,
	BYTE_W_1,
	BYTE_W_2,
	BYTE_W_3
};

enum w_3_bits {
	INDICATOR_DOWN = 0,
	INDICATOR_UP = 1,
	ARRIVAL = 2,
	MOVING = 4,
	SPEECH_SUPPORT = 5,
	NO_RESPONSE = 7
};

#define ARROW_MASK 0x03;

/// Structure of Header for transmitting data
static CAN_TxHeaderTypeDef tx_header;

/// Structure of Header for receiving data
static CAN_RxHeaderTypeDef rx_header;

/// Buffer for transmitting data
static uint8_t tx_data_can[8] = { 0, };

/// Buffer for receiving data
static uint8_t rx_data_can[6] = { 0x00, };

/// MAilbox for transmitted data
static uint32_t tx_mailbox = 0;

/// String OK
char *str_ok = "OK";

volatile uint32_t alive_cnt[2] = { 0, };

volatile bool is_can_connected = false;

/// Flag to control if data is received
volatile bool is_data_received = false;

/**
 * @brief  Handle Interrupt by receiving data after transmitting by CAN,
 * setting the state when data with StdId is received.
 * @param  hcan: Structure of CAN
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data_can)
			== HAL_OK) {
		if (rx_header.StdId == 46 && rx_header.DLC == 6
				&& rx_data_can[BYTE_CODE_OPERATION_0] == 0x81
				&& rx_data_can[BYTE_CODE_OPERATION_1] == 0x00) {
			alive_cnt[0] = (alive_cnt[0] < UINT32_MAX) ? alive_cnt[0] + 1 : 0;
			is_data_received = true;
			is_can_connected = true;
		}
	}
}

/// Flag to control CAN errors
volatile uint8_t cnt = 0;

/**
 * @brief  Handle Interrupt by CAN errors.
 * @param  hcan: Structure of CAN
 * @retval None
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
	cnt = (cnt < UINT8_MAX) ? cnt + 1 : 0;
	uint32_t er = HAL_CAN_GetError(hcan);
}
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void) {

	/* USER CODE BEGIN CAN_Init 0 */
	CAN_FilterTypeDef canFilterConfig;
	/* USER CODE END CAN_Init 0 */

	/* USER CODE BEGIN CAN_Init 1 */

	/* USER CODE END CAN_Init 1 */
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 10;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = ENABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = ENABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN CAN_Init 2 */

	canFilterConfig.FilterBank = 0;
	canFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST; //CAN_FILTERMODE_IDMASK; //CAN_FILTERMODE_IDLIST; // CAN_FILTERMODE_IDMASK;
	canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfig.FilterIdHigh = 46 << 5; // 0x0000;  // 46
	canFilterConfig.FilterIdLow = 0x0000;
	canFilterConfig.FilterMaskIdHigh = 46 << 5; //0x07FF << 5; // 0x0000; // 0x002E << 5; // 46
	canFilterConfig.FilterMaskIdLow = 0x0000;
	canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfig.FilterActivation = ENABLE;
	canFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan, &canFilterConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (canHandle->Instance == CAN1) {
		/* USER CODE BEGIN CAN1_MspInit 0 */

		/* USER CODE END CAN1_MspInit 0 */
		/* CAN1 clock enable */
		__HAL_RCC_CAN1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**CAN GPIO Configuration
		 PA11     ------> CAN_RX
		 PA12     ------> CAN_TX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* CAN1 interrupt Init */
		HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
		HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
		HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
		/* USER CODE BEGIN CAN1_MspInit 1 */

		/* USER CODE END CAN1_MspInit 1 */
	}
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle) {

	if (canHandle->Instance == CAN1) {
		/* USER CODE BEGIN CAN1_MspDeInit 0 */

		/* USER CODE END CAN1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_CAN1_CLK_DISABLE();

		/**CAN GPIO Configuration
		 PA11     ------> CAN_RX
		 PA12     ------> CAN_TX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

		/* CAN1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
		HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
		HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
		/* USER CODE BEGIN CAN1_MspDeInit 1 */

		/* USER CODE END CAN1_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */

/**
 * @brief  Setting frame for transmitting TxData by CAN.
 * @param  stdId The standard ID of the frame.
 * @retval None
 */
static void setFrame(uint32_t stdId) {
	tx_header.StdId = stdId;
	tx_header.ExtId = 0;
	tx_header.RTR = CAN_RTR_DATA; //CAN_RTR_REMOTE
	tx_header.IDE = CAN_ID_STD;   // CAN_ID_EXT
	tx_header.DLC = 6;
	tx_header.TransmitGlobalTime = 0;

//	tx_data_can[0] = ;
//	tx_data_can[1] = ;
//	tx_data_can[2] = ;
//	tx_data_can[3] = ;
//	tx_data_can[4] = ;
//	tx_data_can[5] = ;
}

/**
 * @brief  Start CAN. Activate notifications for interrupt callbacks
 * @param  *hcan Pointer on CAN structure.
 * @retval None
 */
void CAN_Start(CAN_HandleTypeDef *hcan) {
	HAL_CAN_Start(hcan);
	HAL_CAN_ActivateNotification(hcan,
			CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF
					| CAN_IT_LAST_ERROR_CODE);
}

/**
 * @brief  Transmit data by CAN.
 * If transmitted data is received then set symbols to matrix.
 * @param  stdId Standard ID of frame.
 * @retval None
 */
void CAN_TxData(uint32_t stdId) {
	setFrame(stdId);
	if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 0) {
		HAL_CAN_AddTxMessage(&hcan, &tx_header, tx_data_can, &tx_mailbox);
	}

	if (is_data_received) {
		is_data_received = false;
		draw_str_on_matrix(str_ok);
	}

}

void CAN_RxData() {

	uint8_t byte_w3_bin[8] = { 0, };
	char temp_str[3];

	if (is_data_received) {
		is_data_received = false;

		convert_number_from_dec_to_bin(rx_data_can[BYTE_W_3], byte_w3_bin, 7);


		direction_t direction = rx_data_can[BYTE_W_3] & ARROW_MASK;
		setting_symbols_floor(temp_str, rx_data_can[BYTE_W_2], direction);
//	switch (direction) {
//	case UPWARD:
//
//		break;
//	case DOWNWARD:
//		str[0] = '<';
//		break;
//	case STOP:
//		str[0] = 'c';
//		break;
//	}


//		if (byte_w3_bin[INDICATOR_DOWN] == 1) {
//			setting_symbols_floor(temp_str, rx_data_can[BYTE_W_2], DOWNWARD);
//		} else
//
//		if (byte_w3_bin[INDICATOR_UP] == 1 ) {
//			setting_symbols_floor(temp_str, rx_data_can[BYTE_W_2], UPWARD);
//		} else
//
//		if (byte_w3_bin[INDICATOR_DOWN] == 0
//				&& byte_w3_bin[INDICATOR_UP] == 0) {
////		if (bin[ARRIVAL] == 1) { // прибытие
//			setting_symbols_floor(temp_str, rx_data_can[BYTE_W_2], STOP);
//
//		} else {
////			setting_symbols_floor(temp_str, rx_data_can[BYTE_W_2], STOP);
//		}

		// while new 6 data bytes are not received, draw str
		while (is_data_received == false && is_can_connected == true) {
			draw_str_on_matrix(temp_str);
		}

	}

}
/* USER CODE END 1 */
