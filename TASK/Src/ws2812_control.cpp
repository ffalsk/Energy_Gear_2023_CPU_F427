#include "ws2812_control.h"
#define Arrow_Pixel_Num 189
#define U_shape_Pixel_Num 251 // 可能每片灯数量不太一样
uint8_t Control_flag[5];
uint8_t ID_setNUM[5] = {0};

UBaseType_t stack_size[10];
WS2812 Arrow_LED1(Arrow_Pixel_Num, &htim5, TIM_CHANNEL_4, 90000000, TIM_CCR_32Bit);
WS2812 U_shape_LED1(U_shape_Pixel_Num, &htim5, TIM_CHANNEL_3, 90000000, TIM_CCR_32Bit);

WS2812 Arrow_LED2(Arrow_Pixel_Num, &htim8, TIM_CHANNEL_4, 180000000, TIM_CCR_16Bit);
WS2812 U_shape_LED2(U_shape_Pixel_Num, &htim8, TIM_CHANNEL_3, 180000000, TIM_CCR_16Bit);

WS2812 Arrow_LED3(Arrow_Pixel_Num, &htim8, TIM_CHANNEL_1, 180000000, TIM_CCR_16Bit);
WS2812 U_shape_LED3(U_shape_Pixel_Num, &htim1, TIM_CHANNEL_3, 180000000, TIM_CCR_32Bit);

WS2812 Arrow_LED4(Arrow_Pixel_Num, &htim2, TIM_CHANNEL_2, 90000000, TIM_CCR_32Bit);
WS2812 U_shape_LED4(U_shape_Pixel_Num, &htim4, TIM_CHANNEL_3, 90000000, TIM_CCR_16Bit);

WS2812 Arrow_LED5(Arrow_Pixel_Num, &htim5, TIM_CHANNEL_1, 90000000, TIM_CCR_32Bit);
WS2812 U_shape_LED5(U_shape_Pixel_Num, &htim1, TIM_CHANNEL_1, 180000000, TIM_CCR_16Bit);

void LED_Control(uint8_t target, uint8_t mode, uint8_t None)
{
    Control_flag[target - 1] = mode;
    if (None != 0)
    {
        ID_setNUM[target - 1] = None;
    }
}

void Arrow_Control(WS2812 &target, uint8_t LED_num)
{
    if (Control_flag[LED_num - 1] == Inactive_Mode) // 待激活模式
    {
        for (int8_t i = 20; i >= -15; i -= 7)
        {
            for (uint8_t j = 0; j < 5; j++)
            {
                target.SetGroupColor(RM_R, RM_G, RM_B, i + 42 * j, 21, -1);
            }
            target.show();
            osDelay(60);
            target.SetALLColor(0, 0, 0);
        }
    }
    else if (Control_flag[LED_num - 1] == Active_Mode) // 已激活模式
    {
        target.SetALLColor(RM_R, RM_G, RM_B);
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == Close_Mode) // 关闭模式
    {
        target.CloseAll();
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == Start_Mode)
    {
        target.FX_RainbowRoll();
    }
    else if (Control_flag[LED_num - 1] == Error_Mode)
    {
        target.SetALLColor(255, 0, 0);
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == IDset_Mode)
    {
        target.SetGroupColor(0, 0, 255, 42 * (ID_setNUM[LED_num - 1] - 1), 21, -1);
        target.show();
        osDelay(60);
    }
}
void U_shape_Control(WS2812 &target, uint8_t LED_num)
{
    if (Control_flag[LED_num - 1] == Inactive_Mode) // 待激活模式
    {
        target.SetGroupColor(RM_R, RM_G, RM_B, 0, 133, 1); //! 需要确定具体数值
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == Active_Mode) // 已激活模式
    {
        target.SetALLColor(RM_R, RM_G, RM_B);
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == Close_Mode) // 关闭模式
    {
        target.CloseAll();
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == Start_Mode)
    {
        target.FX_RainbowRoll();
    }
    else if (Control_flag[LED_num - 1] == Error_Mode)
    {
        target.SetALLColor(255, 0, 0);
        target.show();
        osDelay(60);
    }
    else if (Control_flag[LED_num - 1] == IDset_Mode)
    {
        target.SetGroupColor(0, 0, 255, 42 * (ID_setNUM[LED_num - 1] - 1), 21, -1);
        target.show();
        osDelay(60);
    }
}
void Circle_control(void)
{
    /*
    extern bool key_Press[11];
    uint8_t LED_position[10][2] = {{0, 1},
                                   {1, 6},
                                   {7, 6},
                                   {13, 6},
                                   {19, 8},
                                   {27, 8},
                                   {35, 8},
                                   {43, 8},
                                   {51, 12},
                                   {63, 12}};
    for (uint8_t i = 1; i <= 10; i++)
    {
        if (key_Press[i] == true)
        {

            usart1_printf("%d\n", i);
            Circle_LED.SetGroupColor(RM_R, RM_G, RM_B, LED_position[10 - i][0], LED_position[10 - i][1], 1);
            key_Press[i] = false;
        }
    }

    Circle_LED.show();
    osDelay(70);
    Circle_LED.CloseAll();*/
}
/*--------------------------------- 任务区 ----------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
    /*--------------------------------- 1号 ----------------------------------*/
    void ArrowLED1_control(void *argument)
    {
        Arrow_LED1.Init();

        while (1)
        {
            stack_size[0] = uxTaskGetStackHighWaterMark(NULL);
            Arrow_Control(Arrow_LED1, 1);
        }
    }

    void U_shapeLED1_control(void const *argument)
    {
        U_shape_LED1.Init();
        while (1)
        {
            stack_size[1] = uxTaskGetStackHighWaterMark(NULL);
            U_shape_Control(U_shape_LED1, 1);
        }
    }
    /*--------------------------------- 2号 ----------------------------------*/
    void ArrowLED2_control(void *argument)
    {
        Arrow_LED2.Init();

        while (1)
        {
            stack_size[2] = uxTaskGetStackHighWaterMark(NULL);
            Arrow_Control(Arrow_LED2, 2);
        }
    }

    void U_shapeLED2_control(void const *argument)
    {
        U_shape_LED2.Init();
        while (1)
        {
            stack_size[3] = uxTaskGetStackHighWaterMark(NULL);
            U_shape_Control(U_shape_LED2, 2);
        }
    }
    /*--------------------------------- 3号 ----------------------------------*/
    void ArrowLED3_control(void *argument)
    {
        Arrow_LED3.Init();

        while (1)
        {
            stack_size[4] = uxTaskGetStackHighWaterMark(NULL);
            Arrow_Control(Arrow_LED3, 3);
        }
    }
    void U_shapeLED3_control(void const *argument)
    {
        U_shape_LED3.Init();
        while (1)
        {
            stack_size[5] = uxTaskGetStackHighWaterMark(NULL);
            U_shape_Control(U_shape_LED3, 3);
        }
    }
    /*--------------------------------- 4号 ----------------------------------*/
    void ArrowLED4_control(void *argument)
    {
        Arrow_LED4.Init();

        while (1)
        {
            stack_size[6] = uxTaskGetStackHighWaterMark(NULL);
            Arrow_Control(Arrow_LED4, 4);
        }
    }
    void U_shapeLED4_control(void const *argument)
    {
        U_shape_LED4.Init();
        while (1)
        {
            stack_size[7] = uxTaskGetStackHighWaterMark(NULL);
            U_shape_Control(U_shape_LED4, 4);
        }
    }
    /*--------------------------------- 5号 ----------------------------------*/
    void ArrowLED5_control(void *argument)
    {
        Arrow_LED5.Init();

        while (1)
        {
            stack_size[8] = uxTaskGetStackHighWaterMark(NULL);
            Arrow_Control(Arrow_LED5, 5);
        }
    }
    void U_shapeLED5_control(void const *argument)
    {
        U_shape_LED5.Init();
        while (1)
        {
            stack_size[9] = uxTaskGetStackHighWaterMark(NULL);
            U_shape_Control(U_shape_LED5, 5);
        }
    }
#ifdef __cplusplus
}
#endif
/*--------------------------- 回调函数!新增对象时需要添加对应通道 -----------------------------*/
/***
 * @brief TC回调函数
 * @return	  void
 ***/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    /*--------------------------------- 1号 ----------------------------------*/
    if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        Arrow_LED1.TCcallback();
    }
    else if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED1.TCcallback();
    }
    /*--------------------------------- 2号 ----------------------------------*/
    else if (htim->Instance == TIM8 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        Arrow_LED2.TCcallback();
    }
    else if (htim->Instance == TIM8 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED2.TCcallback();
    }
    /*--------------------------------- 3号 ----------------------------------*/
    else if (htim->Instance == TIM8 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        Arrow_LED3.TCcallback();
    }
    else if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED3.TCcallback();
    }
    /*--------------------------------- 4号 ----------------------------------*/
    else if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        Arrow_LED4.TCcallback();
    }
    else if (htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED4.TCcallback();
    }
    /*--------------------------------- 5号 ----------------------------------*/
    else if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        Arrow_LED5.TCcallback();
    }
    else if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        U_shape_LED5.TCcallback();
    }
}
/***
 * @brief HC回调函数
 * @return	  void
 ***/
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    /*--------------------------------- 1号 ----------------------------------*/
    if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        Arrow_LED1.HCcallback();
    }
    else if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED1.HCcallback();
    }
    /*--------------------------------- 2号 ----------------------------------*/
    else if (htim->Instance == TIM8 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        Arrow_LED2.HCcallback();
    }
    else if (htim->Instance == TIM8 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED2.HCcallback();
    }
    /*--------------------------------- 3号 ----------------------------------*/
    else if (htim->Instance == TIM8 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        Arrow_LED3.HCcallback();
    }
    else if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED3.HCcallback();
    }
    /*--------------------------------- 4号 ----------------------------------*/
    else if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        Arrow_LED4.HCcallback();
    }
    else if (htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        U_shape_LED4.HCcallback();
    }
    /*--------------------------------- 5号 ----------------------------------*/
    else if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        Arrow_LED5.HCcallback();
    }
    else if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        U_shape_LED5.HCcallback();
    }
}
