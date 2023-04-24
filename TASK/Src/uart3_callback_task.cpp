#include "uart3_callback_task.h"
bool print_flag = false;
#ifdef __cplusplus
extern "C"
{
#endif
    void Uart3_callback_task(void *argument)
    {
        while (1)
        {
            if (print_flag && StartUP)
            {
                for (uint8_t i = 0; i < 5; i++)
                {
                    usart3_printf("%d ", flabellum[i].knock_position);
                }
                usart3_printf("\n");
                print_flag = true;
            }
            osDelay(15);
        }
    }
#ifdef __cplusplus
}
#endif