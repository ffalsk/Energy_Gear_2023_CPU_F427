#ifndef __WS2812_CONTROL_H
#define __WS2812_CONTROL_H

#ifdef __cplusplus
#include "main.h"

#define C1 0xff00ff
#define RM_R 255
#define RM_G 7
#define RM_B 7

typedef enum
{
    Close_Mode = 0x00,
    Active_Mode = 0x01,
    Inactive_Mode = 0x02,
    Start_Mode = 0x03,
    Error_Mode = 0x04,
    IDset_Mode = 0x05,
} LED_Mode;
void LED_Control(uint8_t target, uint8_t mode, uint8_t ID_num);

#endif
#endif
