/**
 ******************************************************************************
 * Copyright (c) 2023 - ~, RM-Alliance Development Team
 * @file    ws2812.h
 * @author  何东宸
 * @date    2023.2.3
 * @version 2.1
 * @brief   采用DMA类双缓冲节省空间，支持大量ws2812控制
 *          !需要c++11支持
 *          !采用AC6编译器
 *          !如无必要请勿修改此文件
 * ------------------------------ Change Log ----------------------------------
 *  Date            Version       Description
 *  2023.2.10       1.0           初始版本,加入DMA双缓冲
 *  2023.3.14       2.0           加入C++类支持，多通道控制更加方便.
 *                                @note 目前回调函数未实现解耦合，需要在定义类对象时在回调函数添加对应通道.
 *  2023.3.20       2.1           完善控制函数，加入HSV控制.
 *  2023.4.17       2.2           加入32位定时器通道支持.不想支持F1芯片，F1系列请自行探索
 * ----------------------------------------------------------------------------
 * @usage
 * 1.使用前先创建WS2812对象
 * 2.灯的颜色设置完成后使用show()进行刷新
 * 3.创建对象后要在HAL_TIM_PWM_PulseFinishedCallback和HAL_TIM_PWM_PulseFinishedHalfCpltCallback中添加对应判断
 ******************************************************************************
 **/
#include "ws2812.h"

/***
 * @brief 设置亮度值
 * @param b 设置的亮度值(0-255)
 * @return	  void
 ***/
void WS2812::SetBrightness(uint8_t b)
{
    brightness = b / 255.0f;
}
/***
 * @brief 设置某个灯的RGB值
 * @param R 设置某个灯的R值
 * @param G 设置某个灯的G值
 * @param B 设置某个灯的B值
 * @param led_id 灯编号
 * @return	  void
 ***/
void WS2812::SetPixelColor(uint8_t R, uint8_t G, uint8_t B, int16_t led_id)
{
    // 异常数据处理
    if (led_id >= (Pixel_num) || led_id < 0)
    {
        return;
    }
    if (brightness)
    { // See notes in setBrightness()
        pixel[led_id].R = R * brightness;
        pixel[led_id].G = G * brightness;
        pixel[led_id].B = B * brightness;
    }
    else
    {
        pixel[led_id].R = R;
        pixel[led_id].G = G;
        pixel[led_id].B = B;
    }
}
/*!
  @brief   Set a pixel's color using a 32-bit 'packed' RGB  value.
  @param   c  32-bit color value. next is red, then green,
              and least significant byte is blue.
  @param   led_id  Pixel index, starting from 0.
*/
void WS2812::SetPixelColor(uint32_t c, int16_t led_id)
{
    // 异常数据处理
    if (led_id >= (Pixel_num) || led_id < 0)
    {
        return;
    }
    if (brightness)
    { // See notes in setBrightness()
        pixel[led_id].R = (uint8_t)(c >> 16) * brightness;
        pixel[led_id].G = (uint8_t)(c >> 8) * brightness;
        pixel[led_id].B = (uint8_t)c * brightness;
    }
    else
    {
        pixel[led_id].R = (uint8_t)(c >> 16);
        pixel[led_id].G = (uint8_t)(c >> 8);
        pixel[led_id].B = (uint8_t)c;
    }
}
/***
 * @brief 以HSV模式设置单个灯颜色
 * @param h 0~360
 * @param s 0~100
 * @param v 0~100
 * @param   led_id  Pixel index, starting from 0.
 * @return	  void
 ***/
void WS2812::SetPixelColor_HSV(uint16_t h, uint16_t s, uint16_t v, int16_t led_id)
{
    SetPixelColor(HSVtoRGB(h, s, v), led_id);
}
/***
 * @brief 刷新LED显示
 * @return	  void
 ***/
void WS2812::show(void)
{
    if (DMA_Finished) // 等待上一轮传输完成
    {
        DMA_Finished = false;
        if (CCR_Bits == TIM_CCR_16Bit)
        {
            memset(ws2812_DMA_data, 0, PIXEL_DATA_SIZE * 2);
            HAL_TIM_PWM_Start_DMA(TIM, TIM_channel, (uint32_t *)ws2812_DMA_data, (PIXEL_DATA_LEN * 2));
        }
        else
        {
            memset(ws2812_DMA_data_32bit, 0, PIXEL_DATA_SIZE_32Bit * 2);
            HAL_TIM_PWM_Start_DMA(TIM, TIM_channel, (uint32_t *)ws2812_DMA_data_32bit, (PIXEL_DATA_LEN * 2));
        }

#ifdef configUSE_RTOS
        osDelay(1);
#else
        HAL_Delay(1);
#endif
    }
}
/***
 * @brief 设置某个灯的RGB值
 * @param R 设置所有灯的R值
 * @param G 设置所有灯的G值
 * @param B 设置所有灯的B值
 * @return	  void
 ***/
void WS2812::SetALLColor(uint8_t R, uint8_t G, uint8_t B)
{
    for (uint16_t i = 0; i < Pixel_num; i++)
    {
        SetPixelColor(R, G, B, i);
    }
}
/***
 * @brief 关闭所有灯
 * @return	  void
 ***/
void WS2812::CloseAll(void)
{
    SetALLColor(0, 0, 0);
    show();
}
/***
 * @brief 设置某个灯以及往后或往前的灯的RGB值
 * @param R 设置某个灯的R值
 * @param G 设置某个灯的G值
 * @param B 设置某个灯的B值
 * @param Index 设置某个灯的位置
 * @param num 往后或往前设置多少个灯
 * @param derection 位置的方向 1后-1前
 * @return	  void
 ***/
void WS2812::SetGroupColor(uint8_t R, uint8_t G, uint8_t B, int16_t Index, uint16_t num, int8_t derection)
{
    if (derection == 1) // 向后
    {
        for (int16_t i = Index; i < Index + num; i++)
        {
            SetPixelColor(R, G, B, i);
        }
    }
    else // 向前
    {
        for (int16_t i = Index; i > Index - num; i--)
        {
            SetPixelColor(R, G, B, i);
        }
    }
}
/***
 * @brief 设置某个灯以及往后或往前的灯的RGB值，32bit
 * @param c 32-bit color value
 * @param Index 设置某个灯的位置
 * @param num 往后或往前设置多少个灯
 * @param derection 位置的方向 1后-1前
 * @return	  void
 ***/
void WS2812::SetGroupColor(uint32_t c, int16_t Index, uint16_t num, int8_t derection)
{
    if (derection == 1) // 向后
    {
        for (int16_t i = Index; i < Index + num; i++)
        {
            SetPixelColor(c, i);
        }
    }
    else // 向前
    {
        for (int16_t i = Index; i > Index - num; i--)
        {
            SetPixelColor(c, i);
        }
    }
}

/***
* @brief   scale down a RGB to N 256ths of it's current brightness, using
        plain math' dimming rules, which means that if the low light levels
        may dim all the way to 100% black.
* @param scale 缩放系数 0~255
* @param led_id  Pixel index, starting from 0.
* @return	  void
***/
void WS2812::FadePixel(uint8_t scale, int16_t led_id)
{
    // 异常数据处理
    if (led_id >= (Pixel_num) || led_id < 0)
    {
        return;
    }
    pixel[led_id].R = ((uint16_t)pixel[led_id].R * scale) >> 8;
    pixel[led_id].G = ((uint16_t)pixel[led_id].G * scale) >> 8;
    pixel[led_id].B = ((uint16_t)pixel[led_id].B * scale) >> 8;
}
void WS2812::FadeAll(uint8_t scale)
{
    for (uint16_t i = 0; i < Pixel_num; i++)
    {
        FadePixel(scale, i);
    }
}
void WS2812::FX_RainbowRoll(uint16_t Delay_ms)
{
    uint8_t hue = 0;
    for (uint16_t i = 0; i < Pixel_num; i++)
    {
        SetPixelColor_HSV(hue++, 100, 100, i);
        show();
#ifdef configUSE_RTOS
        osDelay(Delay_ms);
#else
        HAL_Delay(Delay_ms);
#endif
        FadeAll(252);
    }
    for (uint16_t i = Pixel_num - 1; i > 0; i--)
    {
        SetPixelColor_HSV(hue++, 100, 100, i);
        show();
#ifdef configUSE_RTOS
        osDelay(Delay_ms);
#else
        HAL_Delay(Delay_ms);
#endif
        FadeAll(252);
    }
}
void WS2812::FX_SingleScan(uint8_t R, uint8_t G, uint8_t B)
{
    for (uint16_t i = 0; i < Pixel_num; i++)
    {
        SetPixelColor(R, G, B, i);
        show();
#ifdef configUSE_RTOS
        osDelay(100);
#else
        HAL_Delay(100);
#endif
        CloseAll();
    }
}
uint16_t WS2812::GetPixelNum(void)
{
    return Pixel_num;
}
/***
 * @brief 初始化对象
 * @return	  void
 ***/
WS2812::WS2812(uint16_t Pixel_n, TIM_HandleTypeDef *TIM_i, uint16_t TIM_channel_i, uint32_t TIM_clock_i, TIM_CCR_Bits TIM_CCR_Bits_i)
{
    Pixel_num = Pixel_n;
    TIM = TIM_i;
    TIM_channel = TIM_channel_i;
    TIM_clock = TIM_clock_i;
    CCR_Bits = TIM_CCR_Bits_i;
    pixel = new pixel_t[Pixel_num];
    Freq_LED = TIM_clock / 800000;
    //__HAL_TIM_SET_AUTORELOAD(TIM, Freq_LED); // 800KHz
    Pulse_1_code = Freq_LED * 0.7;
    Pulse_0_code = Freq_LED * 0.3;
}
void WS2812::Init(void)
{
    if (CCR_Bits == TIM_CCR_16Bit)
    {
        __HAL_TIM_SET_AUTORELOAD(TIM, Freq_LED); // 800KHz
    }
    else // 32bit
    {
        __HAL_TIM_SET_AUTORELOAD(TIM, (uint32_t)Freq_LED); // 800KHz
    }
}
/***
 * @brief 填充DMA数组
 * @param pixel_next 要填充的灯序号
 * @param *addr 填充数组地址
 * @return	  void
 ***/
void WS2812::pixel_data_fill(uint8_t pixel_next, uint16_t *addr)
{
    // 异常数据处理
    if (pixel_next >= Pixel_num)
    {
        return;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        // 填充数组
        addr[i] = (pixel[pixel_next].G << i) & (0x80) ? Pulse_1_code : Pulse_0_code; // 高位先发
        addr[i + 8] = (pixel[pixel_next].R << i) & (0x80) ? Pulse_1_code : Pulse_0_code;
        addr[i + 16] = (pixel[pixel_next].B << i) & (0x80) ? Pulse_1_code : Pulse_0_code;
    }
}
/***
 * @brief 填充DMA数组32bit
 * @param pixel_next 要填充的灯序号
 * @param *addr 填充数组地址
 * @return	  void
 ***/
void WS2812::pixel_data_fill_32bit(uint8_t pixel_next, uint32_t *addr)
{
    // 异常数据处理
    if (pixel_next >= Pixel_num)
    {
        return;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        // 填充数组
        addr[i] = (uint32_t)(pixel[pixel_next].G << i) & (0x80) ? Pulse_1_code : Pulse_0_code; // 高位先发
        addr[i + 8] = (uint32_t)(pixel[pixel_next].R << i) & (0x80) ? Pulse_1_code : Pulse_0_code;
        addr[i + 16] = (uint32_t)(pixel[pixel_next].B << i) & (0x80) ? Pulse_1_code : Pulse_0_code;
    }
}
void WS2812::TCcallback(void)
{
    cnt++;
    if (cnt <= Pixel_num)
    {
        if (CCR_Bits == TIM_CCR_16Bit)
        {
            pixel_data_fill(cnt - 1, ws2812_DMA_data + PIXEL_DATA_LEN);
        }
        else
        {
            pixel_data_fill_32bit(cnt - 1, ws2812_DMA_data_32bit + PIXEL_DATA_LEN);
        }
    }
    else if (cnt == Pixel_num + 2 - (Pixel_num % 2)) // 奇偶不同
    {
        if (CCR_Bits == TIM_CCR_16Bit)
        {
            memset(ws2812_DMA_data + PIXEL_DATA_LEN, 0, PIXEL_DATA_SIZE);
        }
        else
        {
            memset(ws2812_DMA_data_32bit + PIXEL_DATA_LEN, 0, PIXEL_DATA_SIZE_32Bit);
        }
    }
    else if (cnt >= Pixel_num + 4 + (Pixel_num % 2))
    {
        cnt = 0;
        HAL_TIM_PWM_Stop_DMA(TIM, TIM_channel);
        DMA_Finished = true;
    }
}
void WS2812::HCcallback(void)
{
    cnt++;
    if (cnt <= Pixel_num)
    {
        if (CCR_Bits == TIM_CCR_16Bit)
        {
            pixel_data_fill(cnt - 1, ws2812_DMA_data);
        }
        else
        {
            pixel_data_fill_32bit(cnt - 1, ws2812_DMA_data_32bit);
        }
    }
    else if (cnt == Pixel_num + 1 + (Pixel_num % 2))
    {
        if (CCR_Bits == TIM_CCR_16Bit)
        {
            memset(ws2812_DMA_data, 0, PIXEL_DATA_SIZE);
        }
        else
        {
            memset(ws2812_DMA_data_32bit, 0, PIXEL_DATA_SIZE_32Bit);
        }
    }
}

uint32_t HSVtoRGB(uint16_t h, uint16_t s, uint16_t v)
{
    uint8_t r, g, b;
    // R,G,B from 0-255, H from 0-360, S,V from 0-100
    h %= 256;
    s %= 101;
    v %= 101;
    int i;
    float RGB_min, RGB_max;
    RGB_max = v * 2.55f;
    RGB_min = RGB_max * (100 - s) / 100.0f;

    i = h / 60;
    int difs = h % 60; // factorial part of h

    // RGB adjustment amount by hue
    float RGB_Adj = (RGB_max - RGB_min) * difs / 60.0f;

    switch (i)
    {
    case 0:
        r = RGB_max;
        g = RGB_min + RGB_Adj;
        b = RGB_min;
        break;
    case 1:
        r = RGB_max - RGB_Adj;
        g = RGB_max;
        b = RGB_min;
        break;
    case 2:
        r = RGB_min;
        g = RGB_max;
        b = RGB_min + RGB_Adj;
        break;
    case 3:
        r = RGB_min;
        g = RGB_max - RGB_Adj;
        b = RGB_max;
        break;
    case 4:
        r = RGB_min + RGB_Adj;
        g = RGB_min;
        b = RGB_max;
        break;
    default: // case 5:
        r = RGB_max;
        g = RGB_min;
        b = RGB_max - RGB_Adj;
        break;
    }

    return (uint32_t)(r << 16) + (uint32_t)(g << 8) + b;
}