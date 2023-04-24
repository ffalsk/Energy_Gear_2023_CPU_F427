#include "printf_dma.h"

#define MY_uart huart3
uint8_t _dbg_TXBuff[50]; // printf缓冲区
/***
 * @brief   通过USART1使用DMA发送printf
 * @param *format 要发送的printf数据
 * @return void
 ***/
void usart3_printf(const char *format, ...)
{
    uint32_t length;
    va_list args;

    va_start(args, format);
    length = vsnprintf((char *)_dbg_TXBuff, sizeof(_dbg_TXBuff), (char *)format, args);
    va_end(args);
    // MY_uart.gState = HAL_UART_STATE_READY; // F1 hal库自己不会设置gState值
    HAL_UART_Transmit_DMA(&MY_uart, _dbg_TXBuff, length);
}
