#include "uart3_callback.h"

void Uart3_callback(void)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        usart3_printf("%d ", blades[i].knock_position);
    }
    usart3_printf("\n");
}
