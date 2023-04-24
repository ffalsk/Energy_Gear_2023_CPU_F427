#include "can_handle.h"

CAN_Data_TypeDef blades[5];
volatile static bool CAN1_Tx_Message_Flag = true; // CAN1发送消息标志
bool StartUP = false, DataReceived = false;
uint8_t ReceivedNUM = 0;

void CAN_Data_Decode(CAN_Data_TypeDef *CAN_Data, uint8_t *Rx_Data)
{
    /* Knock                               IDset shake
         ↓                                   ↓     ↓
        └┈┘   └┈┘   └┈┘   └┈┘   └┈┘   └┈┘   └┈┘   └┈┘
    */
    CAN_Data->knock_position = Rx_Data[0];
    // CAN_Data->IDset = Rx_Data[6];
    CAN_Data->hand_shake = Rx_Data[7];
}
/***
 *@brief		CAN1数据接收
 ***/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef CAN1_Rx_Message;
    uint8_t CAN1_Rx_Data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN1_Rx_Message, CAN1_Rx_Data);

    if ((CAN1_Rx_Message.IDE == CAN_ID_STD) && (CAN1_Rx_Message.RTR == CAN_RTR_DATA) && (CAN1_Rx_Message.DLC == 8))
    {
        switch (CAN1_Rx_Message.StdId)
        {
        case 0x101:
        case 0x102:
        case 0x103:
        case 0x104:
        case 0x105:
        {
            CAN_Data_Decode(&blades[CAN1_Rx_Message.StdId - 0x101], CAN1_Rx_Data);
        }
        break;
        default:
        {
            break;
        }
        }
    }

    if (CAN1_Rx_Data[7] == Error_receive)
    {
        blades[CAN1_Rx_Message.StdId - 0x101].StartError = 1;
    }
    if (CAN1_Rx_Data[0] != 0||StartUP==false)
    {
        ReceivedNUM = CAN1_Rx_Message.StdId - 0x100;
    }
    DataReceived = true;
}

/***
 *@brief	CAN1发送
 *@return   void
 ***/
void CAN1_TX_blades(uint16_t ID)
{
    CAN_TxHeaderTypeDef CAN1_Tx_Message;
    uint8_t CAN1_Tx_Data[8];
    uint32_t send_mail_box;
    memset(CAN1_Tx_Data, 0, 8);
    CAN1_Tx_Message.IDE = CAN_ID_STD;   // 标准帧
    CAN1_Tx_Message.RTR = CAN_RTR_DATA; // 数据帧
    CAN1_Tx_Message.DLC = 0x08;         // 帧长度为8
    CAN1_Tx_Message.StdId = ID;         // 帧ID为传入参数的CAN_ID

    CAN1_Tx_Message_Flag = true;
    if (CAN1_Tx_Message_Flag == true)
    {
        CAN1_Tx_Message_Flag = false;
        HAL_CAN_AddTxMessage(&hcan1, &CAN1_Tx_Message, CAN1_Tx_Data, &send_mail_box); // 传输
    }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    CAN1_Tx_Message_Flag = true;
}
void CAN_Filter_config(void) // todo 配置白名单模式，只接收特定的CAN报文
{
    CAN_FilterTypeDef can_Filter = {0};

    can_Filter.FilterIdHigh = 0;
    can_Filter.FilterIdLow = 0;
    can_Filter.FilterMaskIdHigh = 0;
    can_Filter.FilterMaskIdLow = 0;
    can_Filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can_Filter.FilterBank = 0;
    can_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
    can_Filter.FilterScale = CAN_FILTERSCALE_32BIT;
    can_Filter.FilterActivation = CAN_FILTER_ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &can_Filter);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}