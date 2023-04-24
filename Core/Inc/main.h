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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define configUSE_RTOS
#include <stdio.h> //标准输入输出库
#include <math.h>  //数学处理函数
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "rng.h"

#include "printf_dma.h"
#include "uart3_callback.h"

#include "ws2812_control.h"
#include "can_handle_task.h"
#include "main_control_task.h"
#include "ws2812.h"
#include "can_handle.h"

    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */
    // extern void HAL_TIM_PeriodElapsedCallback_User1(TIM_HandleTypeDef *htim);
    extern uint64_t TIM6_tick;
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
#define U_shape2_Pin GPIO_PIN_7
#define U_shape2_GPIO_Port GPIOI
#define Arrow3_Pin GPIO_PIN_5
#define Arrow3_GPIO_Port GPIOI
#define Arrow2_Pin GPIO_PIN_2
#define Arrow2_GPIO_Port GPIOI
#define Arrow1_Pin GPIO_PIN_0
#define Arrow1_GPIO_Port GPIOI
#define U_shape5_Pin GPIO_PIN_8
#define U_shape5_GPIO_Port GPIOA
#define U_shape1_Pin GPIO_PIN_12
#define U_shape1_GPIO_Port GPIOH
#define Arrow5_Pin GPIO_PIN_10
#define Arrow5_GPIO_Port GPIOH
#define U_shape4_Pin GPIO_PIN_14
#define U_shape4_GPIO_Port GPIOD
#define Arrow4_Pin GPIO_PIN_1
#define Arrow4_GPIO_Port GPIOA
#define U_shape3_Pin GPIO_PIN_13
#define U_shape3_GPIO_Port GPIOE

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
