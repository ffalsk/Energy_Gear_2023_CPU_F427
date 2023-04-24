/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ArrowLED1 */
osThreadId_t ArrowLED1Handle;
const osThreadAttr_t ArrowLED1_attributes = {
  .name = "ArrowLED1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for U_shapeLED1 */
osThreadId_t U_shapeLED1Handle;
const osThreadAttr_t U_shapeLED1_attributes = {
  .name = "U_shapeLED1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ArrowLED2 */
osThreadId_t ArrowLED2Handle;
const osThreadAttr_t ArrowLED2_attributes = {
  .name = "ArrowLED2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for U_shapeLED2 */
osThreadId_t U_shapeLED2Handle;
const osThreadAttr_t U_shapeLED2_attributes = {
  .name = "U_shapeLED2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ArrowLED3 */
osThreadId_t ArrowLED3Handle;
const osThreadAttr_t ArrowLED3_attributes = {
  .name = "ArrowLED3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ArrowLED4 */
osThreadId_t ArrowLED4Handle;
const osThreadAttr_t ArrowLED4_attributes = {
  .name = "ArrowLED4",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ArrowLED5 */
osThreadId_t ArrowLED5Handle;
const osThreadAttr_t ArrowLED5_attributes = {
  .name = "ArrowLED5",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for U_shapeLED3 */
osThreadId_t U_shapeLED3Handle;
const osThreadAttr_t U_shapeLED3_attributes = {
  .name = "U_shapeLED3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for U_shapeLED4 */
osThreadId_t U_shapeLED4Handle;
const osThreadAttr_t U_shapeLED4_attributes = {
  .name = "U_shapeLED4",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for U_shapeLED5 */
osThreadId_t U_shapeLED5Handle;
const osThreadAttr_t U_shapeLED5_attributes = {
  .name = "U_shapeLED5",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CAN_handle */
osThreadId_t CAN_handleHandle;
const osThreadAttr_t CAN_handle_attributes = {
  .name = "CAN_handle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MainControl */
osThreadId_t MainControlHandle;
const osThreadAttr_t MainControl_attributes = {
  .name = "MainControl",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ArrowLED1_control(void *argument);
void U_shapeLED1_control(void *argument);
void ArrowLED2_control(void *argument);
void U_shapeLED2_control(void *argument);
void ArrowLED3_control(void *argument);
void ArrowLED4_control(void *argument);
void ArrowLED5_control(void *argument);
void U_shapeLED3_control(void *argument);
void U_shapeLED4_control(void *argument);
void U_shapeLED5_control(void *argument);
void CAN_handle_task(void *argument);
void MainControl_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ArrowLED1 */
  ArrowLED1Handle = osThreadNew(ArrowLED1_control, NULL, &ArrowLED1_attributes);

  /* creation of U_shapeLED1 */
  U_shapeLED1Handle = osThreadNew(U_shapeLED1_control, NULL, &U_shapeLED1_attributes);

  /* creation of ArrowLED2 */
  ArrowLED2Handle = osThreadNew(ArrowLED2_control, NULL, &ArrowLED2_attributes);

  /* creation of U_shapeLED2 */
  U_shapeLED2Handle = osThreadNew(U_shapeLED2_control, NULL, &U_shapeLED2_attributes);

  /* creation of ArrowLED3 */
  ArrowLED3Handle = osThreadNew(ArrowLED3_control, NULL, &ArrowLED3_attributes);

  /* creation of ArrowLED4 */
  ArrowLED4Handle = osThreadNew(ArrowLED4_control, NULL, &ArrowLED4_attributes);

  /* creation of ArrowLED5 */
  ArrowLED5Handle = osThreadNew(ArrowLED5_control, NULL, &ArrowLED5_attributes);

  /* creation of U_shapeLED3 */
  U_shapeLED3Handle = osThreadNew(U_shapeLED3_control, NULL, &U_shapeLED3_attributes);

  /* creation of U_shapeLED4 */
  U_shapeLED4Handle = osThreadNew(U_shapeLED4_control, NULL, &U_shapeLED4_attributes);

  /* creation of U_shapeLED5 */
  U_shapeLED5Handle = osThreadNew(U_shapeLED5_control, NULL, &U_shapeLED5_attributes);

  /* creation of CAN_handle */
  CAN_handleHandle = osThreadNew(CAN_handle_task, NULL, &CAN_handle_attributes);

  /* creation of MainControl */
  MainControlHandle = osThreadNew(MainControl_task, NULL, &MainControl_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ArrowLED1_control */
/**
  * @brief  Function implementing the ArrowLED1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ArrowLED1_control */
__weak void ArrowLED1_control(void *argument)
{
  /* USER CODE BEGIN ArrowLED1_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ArrowLED1_control */
}

/* USER CODE BEGIN Header_U_shapeLED1_control */
/**
* @brief Function implementing the U_shapeLED1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_U_shapeLED1_control */
__weak void U_shapeLED1_control(void *argument)
{
  /* USER CODE BEGIN U_shapeLED1_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END U_shapeLED1_control */
}

/* USER CODE BEGIN Header_ArrowLED2_control */
/**
* @brief Function implementing the ArrowLED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ArrowLED2_control */
__weak void ArrowLED2_control(void *argument)
{
  /* USER CODE BEGIN ArrowLED2_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ArrowLED2_control */
}

/* USER CODE BEGIN Header_U_shapeLED2_control */
/**
* @brief Function implementing the U_shapeLED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_U_shapeLED2_control */
__weak void U_shapeLED2_control(void *argument)
{
  /* USER CODE BEGIN U_shapeLED2_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END U_shapeLED2_control */
}

/* USER CODE BEGIN Header_ArrowLED3_control */
/**
* @brief Function implementing the ArrowLED3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ArrowLED3_control */
__weak void ArrowLED3_control(void *argument)
{
  /* USER CODE BEGIN ArrowLED3_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ArrowLED3_control */
}

/* USER CODE BEGIN Header_ArrowLED4_control */
/**
* @brief Function implementing the ArrowLED4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ArrowLED4_control */
__weak void ArrowLED4_control(void *argument)
{
  /* USER CODE BEGIN ArrowLED4_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ArrowLED4_control */
}

/* USER CODE BEGIN Header_ArrowLED5_control */
/**
* @brief Function implementing the ArrowLED5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ArrowLED5_control */
__weak void ArrowLED5_control(void *argument)
{
  /* USER CODE BEGIN ArrowLED5_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ArrowLED5_control */
}

/* USER CODE BEGIN Header_U_shapeLED3_control */
/**
* @brief Function implementing the U_shapeLED3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_U_shapeLED3_control */
__weak void U_shapeLED3_control(void *argument)
{
  /* USER CODE BEGIN U_shapeLED3_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END U_shapeLED3_control */
}

/* USER CODE BEGIN Header_U_shapeLED4_control */
/**
* @brief Function implementing the U_shapeLED4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_U_shapeLED4_control */
__weak void U_shapeLED4_control(void *argument)
{
  /* USER CODE BEGIN U_shapeLED4_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END U_shapeLED4_control */
}

/* USER CODE BEGIN Header_U_shapeLED5_control */
/**
* @brief Function implementing the U_shapeLED5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_U_shapeLED5_control */
__weak void U_shapeLED5_control(void *argument)
{
  /* USER CODE BEGIN U_shapeLED5_control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END U_shapeLED5_control */
}

/* USER CODE BEGIN Header_CAN_handle_task */
/**
* @brief Function implementing the CAN_handle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_handle_task */
__weak void CAN_handle_task(void *argument)
{
  /* USER CODE BEGIN CAN_handle_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CAN_handle_task */
}

/* USER CODE BEGIN Header_MainControl_task */
/**
* @brief Function implementing the MainControl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainControl_task */
__weak void MainControl_task(void *argument)
{
  /* USER CODE BEGIN MainControl_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END MainControl_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

