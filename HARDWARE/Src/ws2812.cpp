/**
 ******************************************************************************
 * Copyright (c) 2023 - ~, RM-Alliance Development Team
 * @file    ws2812.h
 * @author  �ζ��
 * @date    2023.2.3
 * @version 2.1
 * @brief   ����DMA��˫�����ʡ�ռ䣬֧�ִ���ws2812����
 *          !��Ҫc++11֧��
 *          !����AC6������
 *          !���ޱ�Ҫ�����޸Ĵ��ļ�
 * ------------------------------ Change Log ----------------------------------
 *  Date            Version       Description
 *  2023.2.10       1.0           ��ʼ�汾,����DMA˫����
 *  2023.3.14       2.0           ����C++��֧�֣���ͨ�����Ƹ��ӷ���.
 *                                @note Ŀǰ�ص�����δʵ�ֽ���ϣ���Ҫ�ڶ��������ʱ�ڻص�������Ӷ�Ӧͨ��.
 *  2023.3.20       2.1           ���ƿ��ƺ���������HSV����.
 *  2023.4.17       2.2           ����32λ��ʱ��ͨ��֧��.����֧��F1оƬ��F1ϵ��������̽��
 * ----------------------------------------------------------------------------
 * @usage
 * 1.ʹ��ǰ�ȴ���WS2812����
 * 2.�Ƶ���ɫ������ɺ�ʹ��show()����ˢ��
 * 3.���������Ҫ��HAL_TIM_PWM_PulseFinishedCallback��HAL_TIM_PWM_PulseFinishedHalfCpltCallback����Ӷ�Ӧ�ж�
 ******************************************************************************
 **/
#include "ws2812.h"

/***
 * @brief ��������ֵ
 * @param b ���õ�����ֵ(0-255)
 * @return	  void
 ***/
void WS2812::SetBrightness(uint8_t b)
{
    brightness = b / 255.0f;
}
/***
 * @brief ����ĳ���Ƶ�RGBֵ
 * @param R ����ĳ���Ƶ�Rֵ
 * @param G ����ĳ���Ƶ�Gֵ
 * @param B ����ĳ���Ƶ�Bֵ
 * @param led_id �Ʊ��
 * @return	  void
 ***/
void WS2812::SetPixelColor(uint8_t R, uint8_t G, uint8_t B, int16_t led_id)
{
    // �쳣���ݴ���
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
    // �쳣���ݴ���
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
 * @brief ��HSVģʽ���õ�������ɫ
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
 * @brief ˢ��LED��ʾ
 * @return	  void
 ***/
void WS2812::show(void)
{
    if (DMA_Finished) // �ȴ���һ�ִ������
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
 * @brief ����ĳ���Ƶ�RGBֵ
 * @param R �������еƵ�Rֵ
 * @param G �������еƵ�Gֵ
 * @param B �������еƵ�Bֵ
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
 * @brief �ر����е�
 * @return	  void
 ***/
void WS2812::CloseAll(void)
{
    SetALLColor(0, 0, 0);
    show();
}
/***
 * @brief ����ĳ�����Լ��������ǰ�ĵƵ�RGBֵ
 * @param R ����ĳ���Ƶ�Rֵ
 * @param G ����ĳ���Ƶ�Gֵ
 * @param B ����ĳ���Ƶ�Bֵ
 * @param Index ����ĳ���Ƶ�λ��
 * @param num �������ǰ���ö��ٸ���
 * @param derection λ�õķ��� 1��-1ǰ
 * @return	  void
 ***/
void WS2812::SetGroupColor(uint8_t R, uint8_t G, uint8_t B, int16_t Index, uint16_t num, int8_t derection)
{
    if (derection == 1) // ���
    {
        for (int16_t i = Index; i < Index + num; i++)
        {
            SetPixelColor(R, G, B, i);
        }
    }
    else // ��ǰ
    {
        for (int16_t i = Index; i > Index - num; i--)
        {
            SetPixelColor(R, G, B, i);
        }
    }
}
/***
 * @brief ����ĳ�����Լ��������ǰ�ĵƵ�RGBֵ��32bit
 * @param c 32-bit color value
 * @param Index ����ĳ���Ƶ�λ��
 * @param num �������ǰ���ö��ٸ���
 * @param derection λ�õķ��� 1��-1ǰ
 * @return	  void
 ***/
void WS2812::SetGroupColor(uint32_t c, int16_t Index, uint16_t num, int8_t derection)
{
    if (derection == 1) // ���
    {
        for (int16_t i = Index; i < Index + num; i++)
        {
            SetPixelColor(c, i);
        }
    }
    else // ��ǰ
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
* @param scale ����ϵ�� 0~255
* @param led_id  Pixel index, starting from 0.
* @return	  void
***/
void WS2812::FadePixel(uint8_t scale, int16_t led_id)
{
    // �쳣���ݴ���
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
 * @brief ��ʼ������
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
 * @brief ���DMA����
 * @param pixel_next Ҫ���ĵ����
 * @param *addr ��������ַ
 * @return	  void
 ***/
void WS2812::pixel_data_fill(uint8_t pixel_next, uint16_t *addr)
{
    // �쳣���ݴ���
    if (pixel_next >= Pixel_num)
    {
        return;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        // �������
        addr[i] = (pixel[pixel_next].G << i) & (0x80) ? Pulse_1_code : Pulse_0_code; // ��λ�ȷ�
        addr[i + 8] = (pixel[pixel_next].R << i) & (0x80) ? Pulse_1_code : Pulse_0_code;
        addr[i + 16] = (pixel[pixel_next].B << i) & (0x80) ? Pulse_1_code : Pulse_0_code;
    }
}
/***
 * @brief ���DMA����32bit
 * @param pixel_next Ҫ���ĵ����
 * @param *addr ��������ַ
 * @return	  void
 ***/
void WS2812::pixel_data_fill_32bit(uint8_t pixel_next, uint32_t *addr)
{
    // �쳣���ݴ���
    if (pixel_next >= Pixel_num)
    {
        return;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        // �������
        addr[i] = (uint32_t)(pixel[pixel_next].G << i) & (0x80) ? Pulse_1_code : Pulse_0_code; // ��λ�ȷ�
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
    else if (cnt == Pixel_num + 2 - (Pixel_num % 2)) // ��ż��ͬ
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