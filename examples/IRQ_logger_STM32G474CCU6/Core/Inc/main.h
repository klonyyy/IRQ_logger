/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"

#include "stm32g4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
#define LED_Pin GPIO_PIN_11
#define LED_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOC
#define S3_Pin GPIO_PIN_10
#define S3_GPIO_Port GPIOC
#define S2_Pin GPIO_PIN_11
#define S2_GPIO_Port GPIOC
#define S4_Pin GPIO_PIN_5
#define S4_GPIO_Port GPIOB
#define S1_Pin GPIO_PIN_6
#define S1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LED_ON  LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
#define LED_OFF LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
#define LED_TOG LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

#define LED1_ON  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
#define LED1_OFF LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
#define LED1_TOG LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

#define S1_ON  LL_GPIO_SetOutputPin(S1_GPIO_Port, S1_Pin);
#define S1_OFF LL_GPIO_ResetOutputPin(S1_GPIO_Port, S1_Pin);
#define S1_TOG LL_GPIO_TogglePin(S1_GPIO_Port, S1_Pin);

#define S2_ON  LL_GPIO_SetOutputPin(S2_GPIO_Port, S2_Pin);
#define S2_OFF LL_GPIO_ResetOutputPin(S2_GPIO_Port, S2_Pin);
#define S2_TOG LL_GPIO_TogglePin(S2_GPIO_Port, S2_Pin);

#define S3_ON  LL_GPIO_SetOutputPin(S3_GPIO_Port, S3_Pin);
#define S3_OFF LL_GPIO_ResetOutputPin(S3_GPIO_Port, S3_Pin);
#define S3_TOG LL_GPIO_TogglePin(S3_GPIO_Port, S3_Pin);

#define S4_ON  LL_GPIO_SetOutputPin(S4_GPIO_Port, S4_Pin);
#define S4_OFF LL_GPIO_ResetOutputPin(S4_GPIO_Port, S4_Pin);
#define S4_TOG LL_GPIO_TogglePin(S4_GPIO_Port, S4_Pin);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
