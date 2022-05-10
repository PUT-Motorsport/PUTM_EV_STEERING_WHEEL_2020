///*
// * CAN_driver.c
// *
// *  Created on: 26 kwi 2022
// *      Author: konrad
// */
//#define AQ_ID	0x0A
//#define AQ_DLC	6
//
//
//#include "CAN.h"
//#include "stm32l4p5xx.h"
//
//extern CAN_HandleTypeDef hcan1;
//extern CAN_FilterTypeDef filter;
//extern HAL_StatusTypeDef error_can_status;
//extern CAN_FilterTypeDef sFilterConfig;
//extern CAN_TxHeaderTypeDef tx_header_aq;
//
//
//void can_filter_init(void){
//	sFilterConfig.FilterBank = 0;
//	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
//	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig.FilterIdHigh = 0x0A << 5;
//	sFilterConfig.FilterIdLow = 0x0000;
//	sFilterConfig.FilterMaskIdHigh = 0xFFFF << 5;
//	sFilterConfig.FilterMaskIdLow = 0x0000;
//	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
//	sFilterConfig.FilterActivation = ENABLE;
//	sFilterConfig.SlaveStartFilterBank = 14;
//
//	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
//		Error_Handler();
//	}
//}
//
//void can_tx_header_init(void){
//	tx_header_aq.StdId = AQ_ID;
//	tx_header_aq.RTR = CAN_RTR_DATA;
//	tx_header_aq.IDE = CAN_ID_STD;
//	tx_header_aq.DLC = AQ_DLC;
//	tx_header_aq.TransmitGlobalTime = DISABLE;
//}
//
//
//void can_init(void){
//	can_tx_header_init();
//	can_filter_init();
//
//	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
//		Error_Handler();
//	}
//}
//
//bool can_send_frame(CAN_TxHeaderTypeDef *pHeader, uint8_t aData[], uint32_t *pTxMailbox){
//	if (HAL_CAN_AddTxMessage(&hcan1, &tx_header_aq, aData ,*pTxMailbox) != HAL_OK) {
//		error_can_status = HAL_CAN_AddTxMessage(&hcan1, &tx_header_aq, aData ,*pTxMailbox);
//		//ERROR
//		Error_Handler();
//		return 0;
//	}
//	return 1;
//}
