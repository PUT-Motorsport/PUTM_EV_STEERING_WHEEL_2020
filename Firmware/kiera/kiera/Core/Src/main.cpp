/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "PUTM_EV_CAN_LIBRARY/lib/can_interface.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */
uint32_t timer;

bool sw3_pressed, sw4_pressed, sw5_pressed, sw6_pressed = 0;

int i = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */

  CAN_FilterTypeDef sFilterConfig;
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;


	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
		Error_Handler();
	}

  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  uint32_t timer = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  i++;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (sw3_pressed or sw4_pressed or sw5_pressed or sw6_pressed) {
		  wait_for_second_button();
	  }

	  if (timer + 500 < HAL_GetTick())
	  {
		  heartbeat();
		  timer = HAL_GetTick();
	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 8;
  hcan1.Init.Mode = CAN_MODE_LOOPBACK;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ControlLed4_Pin|ControlLed3_Pin|ControlLed2_Pin|ControlLed1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SW5_Pin */
  GPIO_InitStruct.Pin = SW5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 SW2_1_Pin SW2_2_Pin
                           SW2_3_Pin SW2_4_Pin SW3_Pin SW4_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|SW2_1_Pin|SW2_2_Pin
                          |SW2_3_Pin|SW2_4_Pin|SW3_Pin|SW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_1_Pin SW1_2_Pin SW1_3_Pin SW1_4_Pin */
  GPIO_InitStruct.Pin = SW1_1_Pin|SW1_2_Pin|SW1_3_Pin|SW1_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SW6_Pin */
  GPIO_InitStruct.Pin = SW6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ControlLed4_Pin ControlLed3_Pin ControlLed2_Pin ControlLed1_Pin */
  GPIO_InitStruct.Pin = ControlLed4_Pin|ControlLed3_Pin|ControlLed2_Pin|ControlLed1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	timer = HAL_GetTick();
	PUTM_CAN::Steering_Wheel_event scroll_state{};

	if (GPIO_Pin == SW3_Pin)
	{
		sw3_pressed = 1;
//		HAL_GPIO_TogglePin(ControlLed4_GPIO_Port, ControlLed4_Pin);
	} else if (GPIO_Pin == SW4_Pin)
	{
		sw4_pressed = 1;
//		HAL_GPIO_TogglePin(ControlLed3_GPIO_Port, ControlLed3_Pin);
	} else if (GPIO_Pin == SW5_Pin)
	{
		sw5_pressed = 1;
//		HAL_GPIO_TogglePin(ControlLed2_GPIO_Port, ControlLed2_Pin);
	} else if (GPIO_Pin == SW6_Pin)
	{
		sw6_pressed = 1;
//		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
	} else if (GPIO_Pin == SW1_1_Pin)
	{
		scroll_state.l_s_1 = PUTM_CAN::scrollStates::scroll_1;
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
	} else if (GPIO_Pin == SW1_2_Pin)
	{
		scroll_state.l_s_1 = PUTM_CAN::scrollStates::scroll_2;
		HAL_GPIO_TogglePin(ControlLed2_GPIO_Port, ControlLed2_Pin);
	} else if (GPIO_Pin == SW1_3_Pin)
	{
		scroll_state.l_s_1 = PUTM_CAN::scrollStates::scroll_3;
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
	} else if (GPIO_Pin == SW1_4_Pin)
	{
		scroll_state.l_s_1 = PUTM_CAN::scrollStates::scroll_4;
		HAL_GPIO_TogglePin(ControlLed2_GPIO_Port, ControlLed2_Pin);
	} else if (GPIO_Pin == SW2_1_Pin)
	{
		scroll_state.r_s_1 = PUTM_CAN::scrollStates::scroll_1;
		HAL_GPIO_TogglePin(ControlLed3_GPIO_Port, ControlLed3_Pin);
	} else if (GPIO_Pin == SW2_2_Pin)
	{
		scroll_state.r_s_1 = PUTM_CAN::scrollStates::scroll_2;
		HAL_GPIO_TogglePin(ControlLed4_GPIO_Port, ControlLed4_Pin);
	} else if (GPIO_Pin == SW2_3_Pin)
	{
		scroll_state.r_s_1 = PUTM_CAN::scrollStates::scroll_3;
		HAL_GPIO_TogglePin(ControlLed3_GPIO_Port, ControlLed3_Pin);
	} else if (GPIO_Pin == SW2_4_Pin)
	{
		scroll_state.r_s_1 = PUTM_CAN::scrollStates::scroll_4;
		HAL_GPIO_TogglePin(ControlLed4_GPIO_Port, ControlLed4_Pin);
	}

	auto steering_wheel_frame = PUTM_CAN::Can_tx_message<PUTM_CAN::Steering_Wheel_event>
	(scroll_state, PUTM_CAN::can_tx_header_STEERING_WHEEL_EVENT);

	steering_wheel_frame.send(hcan1);
}

void heartbeat()
{

	PUTM_CAN::Steering_Wheel_main pcb_alive{0, PUTM_CAN::Steering_Wheel_states::OK};

	auto steering_wheel_heartbeat = PUTM_CAN::Can_tx_message<PUTM_CAN::Steering_Wheel_main>
	(pcb_alive, PUTM_CAN::can_tx_header_STEERING_WHEEL_MAIN);

 	steering_wheel_heartbeat.send(hcan1);
}

void wait_for_second_button()
{
	HAL_Delay(500);
	PUTM_CAN::Steering_Wheel_event button_pressed{};

	if (sw3_pressed && sw4_pressed)
	{
		button_pressed.button = PUTM_CAN::buttonStates::button1_2;
	} else if (sw3_pressed && sw5_pressed)
	{
		button_pressed.button = PUTM_CAN::buttonStates::button1_3;

	} else if (sw3_pressed && sw6_pressed)
	{
		button_pressed.button = PUTM_CAN::buttonStates::button1_4;
		HAL_GPIO_TogglePin(ControlLed4_GPIO_Port, ControlLed4_Pin);
	} else if (sw4_pressed && sw5_pressed)
	{
		button_pressed.button = PUTM_CAN::buttonStates::button2_3;
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);

	} else if (sw4_pressed && sw6_pressed)
	{
		button_pressed.button = PUTM_CAN::buttonStates::button2_4;
	} else if (sw5_pressed && sw6_pressed)
	{
		button_pressed.button = PUTM_CAN::buttonStates::button3_4;
	} else if (sw3_pressed) {
		button_pressed.button = PUTM_CAN::buttonStates::button1;
//		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
	} else if (sw4_pressed) {
		button_pressed.button = PUTM_CAN::buttonStates::button2;
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);

	} else if (sw5_pressed) {
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
		button_pressed.button = PUTM_CAN::buttonStates::button3;
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);

	} else if (sw6_pressed) {
		HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
		button_pressed.button = PUTM_CAN::buttonStates::button4;
	}

	auto steering_wheel_frame = PUTM_CAN::Can_tx_message<PUTM_CAN::Steering_Wheel_event>
	(button_pressed, PUTM_CAN::can_tx_header_STEERING_WHEEL_EVENT);


	auto button_status = steering_wheel_frame.send(hcan1);

	if (button_status not_eq HAL_OK) {
		//todo
	}

	reset_flags();

}

//void choose_left_scroll_state()
//{
//	float average_adc = 0;
//
//	//	  calculate average adc
//	for (int i = 0; i < 10; i++) {
//		average_adc = average_adc + left_adc_reading[i];
//	}
//
//	average_adc = average_adc / 10;
//
//	int lss = 0; // Left Scroll State
//
//	if (1110 < average_adc && average_adc < 1115)
//	{
//		lss = 1;
//	} else if (3129 < average_adc && average_adc < 3133)
//	{
//		lss = 2;
//	} else if (3715 < average_adc && average_adc < 3721)
//	{
//		lss = 3;
//	} else if (3970 < average_adc && average_adc < 3975)
//	{
//		lss = 4;
//	}
//
//
//	if (lss != left_last_state)
//	{
//		switch (lss)
//		{
//		case 1:
//			HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
////			HAL_Delay(200);
//			left_last_state = lss;
//			break;
//		case 2:
//			HAL_GPIO_TogglePin(ControlLed2_GPIO_Port, ControlLed2_Pin);
////			HAL_Delay(200);
//			left_last_state = lss;
//			break;
//		case 3:
//			HAL_GPIO_TogglePin(ControlLed3_GPIO_Port, ControlLed3_Pin);
////			HAL_Delay(200);
//			left_last_state = lss;
//			break;
//		case 4:
//			HAL_GPIO_TogglePin(ControlLed4_GPIO_Port, ControlLed4_Pin);
////			HAL_Delay(200);
//			left_last_state = lss;
//			break;
//		}
//	}
//}
//
//void choose_right_scroll_state()
//{
//	float average_adc = 0;
//
//	//	  calculate average adc
//	for (int i = 0; i < 10; i++) {
//		average_adc = average_adc + right_adc_reading[i];
//	}
//
//	average_adc = average_adc / 10;
//
//	int rss = 0; // Right Scroll State
//
//	if (1110 < average_adc && average_adc < 1115)
//	{
//		rss = 1;
//	} else if (3129 < average_adc && average_adc < 3133)
//	{
//		rss = 2;
//	} else if (3715 < average_adc && average_adc < 3721)
//	{
//		rss = 3;
//	} else if (3970 < average_adc && average_adc < 3975)
//	{
//		rss = 4;
//	}
//
//
//	if (rss != right_last_state)
//	{
//		switch (rss)
//		{
//		case 1:
////			HAL_GPIO_TogglePin(ControlLed1_GPIO_Port, ControlLed1_Pin);
////			HAL_Delay(200);
//			right_last_state = rss;
//			break;
//		case 2:
////			HAL_GPIO_TogglePin(ControlLed2_GPIO_Port, ControlLed2_Pin);
////			HAL_Delay(200);
//			right_last_state = rss;
//			break;
//		case 3:
////			HAL_GPIO_TogglePin(ControlLed3_GPIO_Port, ControlLed3_Pin);
////			HAL_Delay(200);
//			right_last_state = rss;
//			break;
//		case 4:
////			HAL_GPIO_TogglePin(ControlLed4_GPIO_Port, ControlLed4_Pin);
////			HAL_Delay(200);
//			right_last_state = rss;
//			break;
//		}
//	}
//}

void reset_flags()
{
	sw3_pressed = 0;
	sw4_pressed = 0;
	sw5_pressed = 0;
	sw6_pressed = 0;
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
