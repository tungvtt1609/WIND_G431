/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOC
#define ADC1_IN1_VOUT_Pin GPIO_PIN_0
#define ADC1_IN1_VOUT_GPIO_Port GPIOA
#define ADC1_IN2_TEMP2_Pin GPIO_PIN_1
#define ADC1_IN2_TEMP2_GPIO_Port GPIOA
#define ADC_IN17_ILOAD_Pin GPIO_PIN_4
#define ADC_IN17_ILOAD_GPIO_Port GPIOA
#define ADC2_IN13_IDUMP_Pin GPIO_PIN_5
#define ADC2_IN13_IDUMP_GPIO_Port GPIOA
#define ADC2_IN3_VIN_Pin GPIO_PIN_6
#define ADC2_IN3_VIN_GPIO_Port GPIOA
#define ADC2_IN4_TEMP1_Pin GPIO_PIN_7
#define ADC2_IN4_TEMP1_GPIO_Port GPIOA
#define Relay1_Pin GPIO_PIN_1
#define Relay1_GPIO_Port GPIOB
#define Relay2_Pin GPIO_PIN_2
#define Relay2_GPIO_Port GPIOB
#define TX_485_Pin GPIO_PIN_10
#define TX_485_GPIO_Port GPIOB
#define RX_485_Pin GPIO_PIN_11
#define RX_485_GPIO_Port GPIOB
#define Control_485_Pin GPIO_PIN_12
#define Control_485_GPIO_Port GPIOB
#define Reset_3g_Pin GPIO_PIN_13
#define Reset_3g_GPIO_Port GPIOB
#define Freq_Pin GPIO_PIN_14
#define Freq_GPIO_Port GPIOB
#define PWM_DUMP_Pin GPIO_PIN_15
#define PWM_DUMP_GPIO_Port GPIOB
#define PWM_BOOST_Pin GPIO_PIN_11
#define PWM_BOOST_GPIO_Port GPIOA
#define PWM_BUCK_Pin GPIO_PIN_12
#define PWM_BUCK_GPIO_Port GPIOA
#define TX_LCD_Pin GPIO_PIN_3
#define TX_LCD_GPIO_Port GPIOB
#define RX_LCD_Pin GPIO_PIN_4
#define RX_LCD_GPIO_Port GPIOB
#define RELAY3_PWM_Pin GPIO_PIN_5
#define RELAY3_PWM_GPIO_Port GPIOB
#define RELAY4_PWM_Pin GPIO_PIN_6
#define RELAY4_PWM_GPIO_Port GPIOB
#define BUZZ_Pin GPIO_PIN_7
#define BUZZ_GPIO_Port GPIOB
#define Power_on_off_3g_Pin GPIO_PIN_9
#define Power_on_off_3g_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
