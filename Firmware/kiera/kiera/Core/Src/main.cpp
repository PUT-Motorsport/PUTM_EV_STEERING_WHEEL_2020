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
#include "etl/list.h"
#include "rotary.hpp"
#include "timer.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DEBUG_LED_MISSION 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */

struct
{
	size_t basic_delay = 10;

	bool rotary_inverted = false;
	bool buttons_inverted = false;
	bool leds_inverted = false;
} config;

const GpioInElement sw1_1(SW1_1_GPIO_Port, SW1_1_Pin, config.rotary_inverted);
const GpioInElement sw1_2(SW1_2_GPIO_Port, SW1_2_Pin, config.rotary_inverted);
const GpioInElement sw1_3(SW1_3_GPIO_Port, SW1_3_Pin, config.rotary_inverted);
const GpioInElement sw1_4(SW1_4_GPIO_Port, SW1_4_Pin, config.rotary_inverted);

const GpioInElement sw2_1(SW2_1_GPIO_Port, SW2_1_Pin, config.rotary_inverted);
const GpioInElement sw2_2(SW2_2_GPIO_Port, SW2_2_Pin, config.rotary_inverted);
const GpioInElement sw2_3(SW2_3_GPIO_Port, SW2_3_Pin, config.rotary_inverted);
const GpioInElement sw2_4(SW2_4_GPIO_Port, SW2_4_Pin, config.rotary_inverted);

const std::array< GpioInElement const*, rotary_pin_count > rot1_arr = { &sw1_1, &sw1_2, &sw1_3, &sw1_4 };
const std::array< GpioInElement const*, rotary_pin_count > rot2_arr = { &sw2_1, &sw2_2, &sw2_3, &sw2_4 };

const GpioInElement sw3(SW3_GPIO_Port, SW3_Pin, config.buttons_inverted);
const GpioInElement sw4(SW4_GPIO_Port, SW4_Pin, config.buttons_inverted);
const GpioInElement sw5(SW5_GPIO_Port, SW5_Pin, config.buttons_inverted);
const GpioInElement sw6(SW6_GPIO_Port, SW6_Pin, config.buttons_inverted);
const GpioInElement sw7(SW7_GPIO_Port, SW7_Pin, config.buttons_inverted);
const GpioInElement sw8(SW8_GPIO_Port, SW8_Pin, config.buttons_inverted);
const GpioInElement sw9(SW9_GPIO_Port, SW9_Pin, config.buttons_inverted);
const GpioInElement sw10(SW10_GPIO_Port, SW10_Pin, config.buttons_inverted);

const GpioOutElement debug_led_1(ControlLed1_GPIO_Port, ControlLed1_Pin, config.leds_inverted);
const GpioOutElement debug_led_2(ControlLed2_GPIO_Port, ControlLed2_Pin, config.leds_inverted);
const GpioOutElement debug_led_3(ControlLed3_GPIO_Port, ControlLed3_Pin, config.leds_inverted);
const GpioOutElement debug_led_4(ControlLed4_GPIO_Port, ControlLed4_Pin, config.leds_inverted);

GpioInElement const &r_button = sw8;
GpioInElement const &g_button = sw6;
GpioInElement const &b_button = sw3;
GpioInElement const &y_button = sw10;

const std::array< GpioInElement const*, 4 > buttons = { &g_button, &r_button, &y_button, &b_button };
const std::array< GpioOutElement const*, 4 > leds = { &debug_led_1, &debug_led_2, &debug_led_3, &debug_led_4 };

etl::list < size_t, 2 > pressed_buttons_i;

Rotary left_rotary(rot1_arr);
Rotary right_rotary(rot2_arr);

struct
{
	bool const& r_btn = r_button.getStateConstRefForDebug();
	bool const& g_btn = g_button.getStateConstRefForDebug();
	bool const& b_btn = b_button.getStateConstRefForDebug();
	bool const& y_btn = y_button.getStateConstRefForDebug();
	RotationDirection const& rot_left = left_rotary.getRotationConstRefForDebug();
	RotationDirection const& rot_right = right_rotary.getRotationConstRefForDebug();

	PUTM_CAN::buttonStates button_states;
	PUTM_CAN::scrollStates right_scroll_state;
	PUTM_CAN::scrollStates left_scroll_state;
} controls_states;

enum struct Mission : uint8_t
{
	Waiting, // waits for triggers
	FirstPressDetected, // waits for release or until second btn pressed
	SecondPressDetected // waits for all buttons to be released
} mission;

CAN_FilterTypeDef can_filtering_config = { 0 };

Timer heartbeat_timer(100);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
void setUpCan();
void heartbeat();
void sendStates(PUTM_CAN::scrollStates rotary_left, PUTM_CAN::scrollStates rotary_right, PUTM_CAN::buttonStates button);
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

   setUpCan();
   debug_led_1.deactivate();
   debug_led_2.deactivate();
   debug_led_3.deactivate();
   debug_led_4.deactivate();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (true)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //----------------------------------------------------------------------------------------------------
	  //handle
//	  if(heartbeat_timer.checkIfTimedOutThenReset())
//		  heartbeat();

	  HAL_Delay(config.basic_delay);

	  for(const auto btn_ptr : buttons) btn_ptr->handle();
	  right_rotary.handle();
	  left_rotary.handle();

#if DEBUG_LED_MISSION == 1
	  if(controls_states.g_btn) debug_led_1.activate();
	  else debug_led_1.deactivate();
	  if(controls_states.r_btn) debug_led_2.activate();
	  else debug_led_2.deactivate();
	  if(controls_states.y_btn) debug_led_3.activate();
	  else debug_led_3.deactivate();
	  if(controls_states.b_btn) debug_led_4.activate();
	  else debug_led_4.deactivate();
#endif
	  //----------------------------------------------------------------------------------------------------
	  // Rotary outside of main logic
	  auto left_rot = left_rotary.getRotation();
	  if(left_rot != RotationDirection::None)
	  {

	  }
	  auto right_rot = right_rotary.getRotation();
	  if(right_rot != RotationDirection::None)
	  {

	  }
	  //----------------------------------------------------------------------------------------------------

	  if(pressed_buttons_i.size() == 0) mission = Mission::Waiting;

	  switch (mission)
	  {
	  	  case Mission::Waiting:
	  	  {
	  		  for (size_t i = 0; i < buttons.size(); i++)
	  		  {
	  			  if(buttons[i]->risingEdge())
	  			  {
	  				  mission = Mission::FirstPressDetected;
	  				  pressed_buttons_i.emplace_back(i);
	  				  break;
	  			  }
	  		  }
	  	  }break;
	  	  case Mission::FirstPressDetected:
	  	  {
	  		  if(buttons[pressed_buttons_i.front()]->fallingEdge())
	  		  {
	  			  size_t state = pressed_buttons_i.front();
	  			  sendStates(PUTM_CAN::scrollStates::scroll_1, PUTM_CAN::scrollStates::scroll_1, PUTM_CAN::buttonStates(state + 1));
	  			  pressed_buttons_i.clear();
	  			  mission = Mission::Waiting;
	  			  break;
	  		  }
	  		  for (size_t i = 0; i < buttons.size(); i++)
			  {
	  			  if(i == pressed_buttons_i.front()) continue;
				  if(buttons[i]->risingEdge())
				  {
					  mission = Mission::SecondPressDetected;
					  pressed_buttons_i.emplace_back(i);

		  			  size_t state_1 = pressed_buttons_i.front();
		  			  size_t state_2 = pressed_buttons_i.back();
		  			  /*
		  			   * calc button states based on the indentifier:
		  			   * first = 0, second = 1 and so on.
		  			   * 1_2 = 4 + first + second = 5
		  			   * 1_3 = 4 + first + third = 6
		  			   * 1_4 = 4 + first + fourth = 7
		  			   * 2_3 = 4 + second + third + (1) = 7 + 1 = 8
		  			   * 2_4 = 4 + second + third + (1) = 8 + 1 = 9;
		  			   * 3_4 = 4 + third + fourth + (1) = 9 + 1 = 10;
		  			   */
		  			  size_t state = state_1 + state_2 + 4;
		  			  if(state >= 7 && state_1 != 0 && state_2 != 0) state += 1;

		  			  sendStates(PUTM_CAN::scrollStates::scroll_1, PUTM_CAN::scrollStates::scroll_1, PUTM_CAN::buttonStates(state));
					  break;
				  }
			  }
	  	  }break;
	  	  case Mission::SecondPressDetected:
		  {
			  for (auto & btn : buttons) btn->fallingEdge();
			  if(!buttons[pressed_buttons_i.front()]->isActive() && !buttons[pressed_buttons_i.back()]->isActive())
			  {
	  			  pressed_buttons_i.clear();
	  			  mission = Mission::Waiting;
			  }
		  }break;
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
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_14TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
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

  /*Configure GPIO pins : SW7_Pin SW8_Pin SW9_Pin SW10_Pin */
  GPIO_InitStruct.Pin = SW7_Pin|SW8_Pin|SW9_Pin|SW10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IMU_INIT1_Pin IMU_INIT2_Pin */
  GPIO_InitStruct.Pin = IMU_INIT1_Pin|IMU_INIT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_1_Pin SW1_2_Pin SW1_3_Pin SW1_4_Pin */
  GPIO_InitStruct.Pin = SW1_1_Pin|SW1_2_Pin|SW1_3_Pin|SW1_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SW2_1_Pin SW2_2_Pin SW2_3_Pin SW2_4_Pin
                           SW3_Pin SW4_Pin SW5_Pin */
  GPIO_InitStruct.Pin = SW2_1_Pin|SW2_2_Pin|SW2_3_Pin|SW2_4_Pin
                          |SW3_Pin|SW4_Pin|SW5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SW6_Pin */
  GPIO_InitStruct.Pin = SW6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ControlLed4_Pin ControlLed3_Pin ControlLed2_Pin ControlLed1_Pin */
  GPIO_InitStruct.Pin = ControlLed4_Pin|ControlLed3_Pin|ControlLed2_Pin|ControlLed1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//do nothing
}

void sendStates(PUTM_CAN::scrollStates rotary_left, PUTM_CAN::scrollStates rotary_right, PUTM_CAN::buttonStates button)
{
	controls_states.button_states = button;
	controls_states.left_scroll_state = rotary_left;
	controls_states.right_scroll_state = rotary_right;

	if(button == PUTM_CAN::buttonStates::button4)
	{
		PUTM_CAN::AQ_ts_button aq_ts_button
		{
			.placeholder = 0
		};

		auto aq_ts_button_frame = PUTM_CAN::Can_tx_message(aq_ts_button, PUTM_CAN::can_tx_header_AQ_TS_BUTTON);

		if(aq_ts_button_frame.send(hcan1) != HAL_OK) Error_Handler();
	}
	else
	{
		PUTM_CAN::Steering_Wheel_event payload
		{
			.button = button,
			.l_s_1 = rotary_left,
			.r_s_1 = rotary_right
		};

		auto steering_wheel_frame = PUTM_CAN::Can_tx_message(payload, PUTM_CAN::can_tx_header_STEERING_WHEEL_EVENT);

		if(steering_wheel_frame.send(hcan1) != HAL_OK) Error_Handler();
	}
}

void heartbeat()
{
	PUTM_CAN::Steering_Wheel_main pcb_alive
	{
		.s_w_a = 0,
		.device_state =PUTM_CAN::Steering_Wheel_states::OK
	};

	auto steering_wheel_heartbeat = PUTM_CAN::Can_tx_message(pcb_alive, PUTM_CAN::can_tx_header_STEERING_WHEEL_MAIN);

 	if(steering_wheel_heartbeat.send(hcan1) != HAL_OK) Error_Handler();
}

void setUpCan()
{
	can_filtering_config.FilterBank = 0;
	can_filtering_config.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filtering_config.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filtering_config.FilterIdHigh = 0x0000;
	can_filtering_config.FilterIdLow = 0x0000;
	can_filtering_config.FilterMaskIdHigh = 0x0000;
	can_filtering_config.FilterMaskIdLow = 0x0000;
	can_filtering_config.FilterFIFOAssignment = CAN_RX_FIFO0;
	can_filtering_config.FilterActivation = ENABLE;
	can_filtering_config.SlaveStartFilterBank = 14;

	if ( HAL_CAN_ConfigFilter(&hcan1, &can_filtering_config) != HAL_OK )
		Error_Handler();

	if ( HAL_CAN_Start(&hcan1) != HAL_OK )
		Error_Handler();

	if ( HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK )
		Error_Handler();
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
  for (auto led : leds) led->deactivate();
  __disable_irq();
  while (1)
  {
	  HAL_Delay(1000);
	  for (auto led : leds) led->toggle();
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
