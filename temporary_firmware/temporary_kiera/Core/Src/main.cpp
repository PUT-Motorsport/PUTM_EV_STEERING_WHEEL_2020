/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>

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

 bool sw3_pressed, sw4_pressed, sw5_pressed, sw6_pressed = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
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
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_IT(&hadc1);

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


   HAL_ADC_Start_DMA(&hadc1, (uint32_t*)left_adc_reading, 10);


   HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
   //  HAL_ADC_Start_IT(&hadc2);
//   HAL_ADC_Start_DMA(&hadc1, (uint32_t*)right_adc_reading, 10);
   uint32_t timer = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_Pin PA8 */
  GPIO_InitStruct.Pin = SW1_Pin|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	timer = HAL_GetTick();

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
	}
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


	auto status = steering_wheel_frame.send(hcan1);

	if (status not_eq HAL_OK) {
		//todo
	}

	reset_flags();

}

void reset_flags()
{
	sw3_pressed = 0;
	sw4_pressed = 0;
	sw5_pressed = 0;
	sw6_pressed = 0;
}
/* USER CODE END 4 */

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
