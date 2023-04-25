#ifndef __WS2812_H
#define __WS2812_H

#ifdef __cplusplus

#include "main.h"

#define PIXEL_DATA_LEN 24  // 每个灯的数据长度
#define PIXEL_DATA_SIZE 48 // 每个灯的数据大小
#define PIXEL_DATA_SIZE_32Bit 96

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} pixel_t;
enum TIM_CCR_Bits
{
    TIM_CCR_8Bit = 0x00,
    TIM_CCR_16Bit = 0x01,
    TIM_CCR_32Bit = 0x02,
};

class WS2812
{
public:
    WS2812(uint16_t Pixel_n, TIM_HandleTypeDef *TIM_i, uint16_t TIM_channel_i, uint32_t TIM_clock_i, TIM_CCR_Bits TIM_CCR_Bits_i);
    void SetPixelColor(uint8_t R, uint8_t G, uint8_t B, int16_t led_id);
    void SetPixelColor(uint32_t c, int16_t led_id);
    void SetPixelColor_HSV(uint16_t h, uint16_t s, uint16_t v, int16_t led_id);
    void SetBrightness(uint8_t b);
    void show(void);
    void SetALLColor(uint8_t R, uint8_t G, uint8_t B);
    void CloseAll(void);
    void SetGroupColor(uint8_t R, uint8_t G, uint8_t B, int16_t Index, uint16_t num, int8_t derection);
    void SetGroupColor(uint32_t c, int16_t Index, uint16_t num, int8_t derection);
    void FadePixel(uint8_t scale, int16_t led_id);
    void FadeAll(uint8_t scale);
    void HCcallback(void);
    void TCcallback(void);
    uint16_t GetPixelNum(void);
    void FX_RainbowRoll(uint16_t Delay_ms);
    void FX_SingleScan(uint8_t R, uint8_t G, uint8_t B);
    void Init(void);

private:
    void pixel_data_fill(uint8_t pixel_next, uint16_t *addr);       /// 填充DMA数组
    void pixel_data_fill_32bit(uint8_t pixel_next, uint32_t *addr); /// 填充DMA数组32bit
    uint16_t ws2812_DMA_data[PIXEL_DATA_LEN * 2] = {0};             // DMA 传输的数据
    uint32_t ws2812_DMA_data_32bit[PIXEL_DATA_LEN * 2] = {0};       // 对32位寄存器的支持
    volatile uint32_t cnt = 0;                                      // DMA 计数器
    pixel_t *pixel;
    TIM_HandleTypeDef *TIM;
    uint16_t TIM_channel;
    uint32_t TIM_clock;
    uint16_t Pulse_1_code, Pulse_0_code;
    float brightness = 1;
    bool DMA_Finished = true;
    uint16_t Pixel_num; // 灯数
    uint16_t Freq_LED;
    TIM_CCR_Bits CCR_Bits;
};

uint32_t HSVtoRGB(uint16_t h, uint16_t s, uint16_t v);

#endif
#endif