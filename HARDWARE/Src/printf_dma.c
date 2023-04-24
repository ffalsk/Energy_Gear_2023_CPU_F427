#include "printf_dma.h"

#define MY_uart huart3
uint8_t _dbg_TXBuff[50]; // printf������
/***
 * @brief   ͨ��USART1ʹ��DMA����printf
 * @param *format Ҫ���͵�printf����
 * @return void
 ***/
void usart3_printf(const char *format, ...)
{
    uint32_t length;
    va_list args;

    va_start(args, format);
    length = vsnprintf((char *)_dbg_TXBuff, sizeof(_dbg_TXBuff), (char *)format, args);
    va_end(args);
    // MY_uart.gState = HAL_UART_STATE_READY; // F1 hal���Լ���������gStateֵ
    HAL_UART_Transmit_DMA(&MY_uart, _dbg_TXBuff, length);
}
