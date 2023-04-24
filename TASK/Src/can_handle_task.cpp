#include "can_handle_task.h"
// UBaseType_t stack_size1;
#ifdef __cplusplus
extern "C"
{
#endif
    void CAN_handle_task(void *argument)
    {
        CAN_Filter_config();
        while (1)
        {

            /*for (uint8_t i = 1; i <= 5; i++)
            {
                CAN1_TX_blades(0x300 + i); // ÐÄÌø°ü
            }*/
            // stack_size1 = uxTaskGetStackHighWaterMark(NULL);
            osDelay(1000);
        }
    }
#ifdef __cplusplus
}
#endif
