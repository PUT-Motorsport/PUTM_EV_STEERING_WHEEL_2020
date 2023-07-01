/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

void choose_left_scroll_state();
float calculate_average_adc();
void reset_flags();
void wait_for_second_button();
void choose_right_scroll_state();
void send_example();
void heartbeat();
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW7_Pin GPIO_PIN_1
#define SW7_GPIO_Port GPIOA
#define SW8_Pin GPIO_PIN_2
#define SW8_GPIO_Port GPIOA
#define IMU_INIT1_Pin GPIO_PIN_4
#define IMU_INIT1_GPIO_Port GPIOC
#define IMU_INIT2_Pin GPIO_PIN_5
#define IMU_INIT2_GPIO_Port GPIOC
#define IMU_INIT2_EXTI_IRQn EXTI9_5_IRQn
#define SW1_1_Pin GPIO_PIN_12
#define SW1_1_GPIO_Port GPIOB
#define SW1_2_Pin GPIO_PIN_13
#define SW1_2_GPIO_Port GPIOB
#define SW1_3_Pin GPIO_PIN_14
#define SW1_3_GPIO_Port GPIOB
#define SW1_4_Pin GPIO_PIN_15
#define SW1_4_GPIO_Port GPIOB
#define SW2_1_Pin GPIO_PIN_6
#define SW2_1_GPIO_Port GPIOC
#define SW2_2_Pin GPIO_PIN_7
#define SW2_2_GPIO_Port GPIOC
#define SW2_3_Pin GPIO_PIN_8
#define SW2_3_GPIO_Port GPIOC
#define SW2_4_Pin GPIO_PIN_9
#define SW2_4_GPIO_Port GPIOC
#define SW9_Pin GPIO_PIN_8
#define SW9_GPIO_Port GPIOA
#define SW10_Pin GPIO_PIN_9
#define SW10_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_10
#define SW3_GPIO_Port GPIOC
#define SW4_Pin GPIO_PIN_11
#define SW4_GPIO_Port GPIOC
#define SW5_Pin GPIO_PIN_12
#define SW5_GPIO_Port GPIOC
#define SW6_Pin GPIO_PIN_2
#define SW6_GPIO_Port GPIOD
#define ControlLed4_Pin GPIO_PIN_4
#define ControlLed4_GPIO_Port GPIOB
#define ControlLed3_Pin GPIO_PIN_5
#define ControlLed3_GPIO_Port GPIOB
#define ControlLed2_Pin GPIO_PIN_6
#define ControlLed2_GPIO_Port GPIOB
#define ControlLed1_Pin GPIO_PIN_7
#define ControlLed1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
