#include "can_handle_task.h"
UBaseType_t stack_size1;

    void CAN_handle_task(void *argument)
    {
        while (1)
        {
            CAN_Filter_config();
            for (uint8_t i = 1; i <= 5; i++)
            {
                CAN1_TX_blades(0x300 + i); // ÐÄÌø°ü
            }
            stack_size1 = uxTaskGetStackHighWaterMark(NULL);
            osDelay(50);
        }
    }
