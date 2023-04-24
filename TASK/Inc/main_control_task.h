#ifndef __MAIN_CONTROL_TASK_H
#define __MAIN_CONTROL_TASK_H

#ifdef __cplusplus
#include "main.h"

extern void LED_Control(uint8_t target, uint8_t mode, uint8_t ID_num);
extern void Uart3_callback(void);
void HAL_TIM_PeriodElapsedCallback_User1(TIM_HandleTypeDef *htim);
#endif
#endif