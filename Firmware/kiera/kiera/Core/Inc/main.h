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
#define SW1_Pin GPIO_PIN_1
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_10
#define SW3_GPIO_Port GPIOC
#define SW3_EXTI_IRQn EXTI15_10_IRQn
#define SW4_Pin GPIO_PIN_11
#define SW4_GPIO_Port GPIOC
#define SW4_EXTI_IRQn EXTI15_10_IRQn
#define SW5_Pin GPIO_PIN_12
#define SW5_GPIO_Port GPIOC
#define SW5_EXTI_IRQn EXTI15_10_IRQn
#define SW6_Pin GPIO_PIN_2
#define SW6_GPIO_Port GPIOD
#define SW6_EXTI_IRQn EXTI2_IRQn
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
