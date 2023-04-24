#ifndef __CAN_HANDLE_H
#define __CAN_HANDLE_H

#ifdef __cplusplus
#include "main.h"

enum
{
    StartUP_shake = 0x00,
    HeartBeat_shake = 0x01,
    OK_receive = 0x02,
    Error_receive = 0x03,
};
typedef struct
{
    uint8_t knock_position;
    uint8_t hand_shake;
    uint8_t StartError;
    uint8_t IDset;
} CAN_Data_TypeDef;
extern CAN_Data_TypeDef blades[5];
extern bool StartUP, DataReceived;
extern uint8_t ReceivedNUM;
void CAN1_TX_blades(uint16_t ID);
void CAN_Filter_config(void);
#endif
#endif