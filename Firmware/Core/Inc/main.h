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
#define PWR_CTRL_Pin GPIO_PIN_5
#define PWR_CTRL_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()    __set_PRIMASK(0)    /* ʹ��ȫ���ж� */
#define DISABLE_INT()   __set_PRIMASK(1)    /* ��ֹȫ���ж� */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
