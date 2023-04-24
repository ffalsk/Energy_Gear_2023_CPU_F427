#include "main_control_task.h"
uint8_t fan_OK_count = 0; // 正常扇叶数目
bool knock[5];            // 击中编号记录
uint64_t TIM6_tick = 0;
bool Is_IDsetMode = false; // ID设置模式
bool Is_startUP[5] = {0};  // 五个扇叶是否启动
bool time_started = false;
uint64_t last_tick = 0;
uint8_t random_num; // 扇叶随机数
// UBaseType_t stack_size2;
int RNG_Get_RandomRange(int min, int max)
{
    uint32_t num_p;
    // HAL_RNG_GenerateRandomNumber(&hrng, num_p);
    num_p = hrng.Instance->DR;//绕过HAL库
    return (num_p) % (max - min + 1) + min;
}

#ifdef __cplusplus
extern "C"
{
#endif
    void MainControl_task(void *argument)
    {
        while (1)
        {
            // stack_size2 = uxTaskGetStackHighWaterMark(NULL);
            if (DataReceived && !StartUP)
            {
                if (blades[ReceivedNUM - 1].StartError == 1) // 错误处理
                {
                    // 红色灯警告
                    LED_Control(ReceivedNUM, Error_Mode, 0);
                    while (1)
                    {
                    };
                }
                else if (blades[ReceivedNUM - 1].hand_shake == OK_receive)
                {
                    // 启动闪灯
                    LED_Control(ReceivedNUM, Start_Mode, 0); //
                    if (Is_startUP[ReceivedNUM - 1] == false)
                    {
                        fan_OK_count++;
                        Is_startUP[ReceivedNUM - 1] = true;
                    }

                    osDelay(500); // 有无必要？
                }
                if (fan_OK_count >= 1) // 5
                {
                    StartUP = true; // 启动完成
                }
                DataReceived = false;
            }
/*-------------------------------- 启动后 ----------------------------------*/
#if 0
            for (uint8_t i = 0; i < 5; i++) // 判断是否进入ID设置
            {
                uint8_t ID_set_count = 0;
                if (blades[i].IDset != 0)
                {
                    ID_set_count++;
                }
                if (i == 4) // 循环结束时判断
                {
                    if (ID_set_count == 0) // 全都没有进入设置
                    {
                        Is_IDsetMode = false;
                    }
                    else
                    {
                        Is_IDsetMode = true;
                    }
                }
            }

            if (DataReceived && StartUP && Is_IDsetMode)
            {
                for (uint8_t i = 0; i < 5; i++)
                {
                    if (blades[i].IDset != 0)
                    {
                        LED_Control(i + 1, IDset_Mode, blades[i].IDset);
                    }
                }
            }
#endif
            if (DataReceived && StartUP && !Is_IDsetMode)
            {

                if (time_started == false)
                {
                    while (1)
                    {
                        if (hrng.State == HAL_RNG_STATE_READY)
                        {
                            random_num = (uint8_t)RNG_Get_RandomRange(1, 5); // 随机数生成
                            if (knock[random_num - 1] == false)              // 不能生成已激活的扇叶
                            {
                                break;
                            }
                        }
                    }
                    // CAN1_TX_blades(0x300 + random_num);        // 发送指令
                    LED_Control(random_num, Inactive_Mode, 0); // 对应亮灯，进入未激活状态

                    Uart3_callback(); // 串口打印

                    // 启动定时器
                    last_tick = TIM6_tick;
                    time_started = true;
                }
                if ((TIM6_tick - last_tick) <= 2500 && ReceivedNUM == random_num) // 正确打中
                {
                    if (knock[random_num] = false)
                    {
                        fan_OK_count++;
                        time_started = false; // 击中后停止计时
                    }
                    knock[random_num] = true;
                    // 亮灯改变
                    LED_Control(random_num, Active_Mode, 0); // 对应亮灯，进入已激活状态
                }
                else if (((TIM6_tick - last_tick) <= 2500 && ReceivedNUM != random_num) || (TIM6_tick - last_tick) >= 2500) // 未正确打中
                {
                    for (uint8_t i = 0; i < 5; i++)
                    {
                        knock[i] = false;
                        // 亮灯改变
                        LED_Control(i, Close_Mode, 0); // 对应灭灯
                        osDelay(100);
                    }
                    if ((TIM6_tick - last_tick) >= 2500)
                    {
                        time_started = false;
                    }
                }
                if (fan_OK_count == 5) // 全部点亮
                {
                    // 闪灯5次
                    for (uint8_t j = 0; j < 5; j++)
                    {
                        knock[j] = false;
                        for (uint8_t i = 0; i < 5; i++)
                        {
                            // 亮灯改变
                            LED_Control(i, Active_Mode, 0);
                        }
                        osDelay(500);
                    }
                    // 逐个熄灭
                    for (uint8_t i = 0; i < 5; i++)
                    {
                        // 亮灯改变
                        LED_Control(i, Close_Mode, 0); // 对应灭灯
                        osDelay(1000);
                    }
                    fan_OK_count = 0;
                    osDelay(2000); // 等待2s，重新开始
                }
                ReceivedNUM = 0;
                DataReceived = false;
            }
            // LED_Control(1, Active_Mode, 0);
            osDelay(10);
        }
    }
#ifdef __cplusplus
}
#endif
