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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "malloc.h"

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
#define CHA_SEL3_Pin GPIO_PIN_3
#define CHA_SEL3_GPIO_Port GPIOE
#define CHB_DCAC_Pin GPIO_PIN_13
#define CHB_DCAC_GPIO_Port GPIOC
#define RELAY_A_Pin GPIO_PIN_14
#define RELAY_A_GPIO_Port GPIOC
#define CHA_DCAC_Pin GPIO_PIN_15
#define CHA_DCAC_GPIO_Port GPIOC
#define EC1A_Pin GPIO_PIN_0
#define EC1A_GPIO_Port GPIOC
#define EC1B_Pin GPIO_PIN_1
#define EC1B_GPIO_Port GPIOC
#define EC2A_Pin GPIO_PIN_2
#define EC2A_GPIO_Port GPIOC
#define EC2B_Pin GPIO_PIN_3
#define EC2B_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_3
#define KEY4_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_6
#define LCD_BL_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_7
#define BEEP_GPIO_Port GPIOA
#define VBAT_Pin GPIO_PIN_4
#define VBAT_GPIO_Port GPIOC
#define PWR_CTRL_Pin GPIO_PIN_5
#define PWR_CTRL_GPIO_Port GPIOC
#define CLK_B_Pin GPIO_PIN_6
#define CLK_B_GPIO_Port GPIOC
#define CLK_A_Pin GPIO_PIN_8
#define CLK_A_GPIO_Port GPIOC
#define RELAY_B_Pin GPIO_PIN_9
#define RELAY_B_GPIO_Port GPIOC
#define CHB_SEL3_Pin GPIO_PIN_8
#define CHB_SEL3_GPIO_Port GPIOA
#define CHG_CTRL_Pin GPIO_PIN_9
#define CHG_CTRL_GPIO_Port GPIOA
#define CHB_SEL2_Pin GPIO_PIN_10
#define CHB_SEL2_GPIO_Port GPIOA
#define CHB_SEL1_Pin GPIO_PIN_10
#define CHB_SEL1_GPIO_Port GPIOC
#define TRIG_CTRL_Pin GPIO_PIN_11
#define TRIG_CTRL_GPIO_Port GPIOC
#define CHB_CTRL_Pin GPIO_PIN_12
#define CHB_CTRL_GPIO_Port GPIOC
#define ADC_S1_Pin GPIO_PIN_2
#define ADC_S1_GPIO_Port GPIOD
#define ADC_S2_Pin GPIO_PIN_3
#define ADC_S2_GPIO_Port GPIOD
#define CHA_CTRL_Pin GPIO_PIN_6
#define CHA_CTRL_GPIO_Port GPIOD
#define CHA_SEL1_Pin GPIO_PIN_0
#define CHA_SEL1_GPIO_Port GPIOE
#define CHA_SEL2_Pin GPIO_PIN_1
#define CHA_SEL2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* 开关全局中断的宏 */
#define ENABLE_INT()    __set_PRIMASK(0)    /* 使能全局中断 */
#define DISABLE_INT()   __set_PRIMASK(1)    /* 禁止全局中断 */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
