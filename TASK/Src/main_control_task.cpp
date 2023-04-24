#include "main_control_task.h"
uint8_t fan_OK_count = 0; // ������Ҷ��Ŀ
bool knock[5];            // ���б�ż�¼
uint64_t TIM6_tick = 0;
bool Is_IDsetMode = false; // ID����ģʽ
bool Is_startUP[5] = {0};  // �����Ҷ�Ƿ�����
bool time_started = false;
uint64_t last_tick = 0;
uint8_t random_num; // ��Ҷ�����
// UBaseType_t stack_size2;
int RNG_Get_RandomRange(int min, int max)
{
    uint32_t num_p;
    // HAL_RNG_GenerateRandomNumber(&hrng, num_p);
    num_p = hrng.Instance->DR;//�ƹ�HAL��
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
                if (blades[ReceivedNUM - 1].StartError == 1) // ������
                {
                    // ��ɫ�ƾ���
                    LED_Control(ReceivedNUM, Error_Mode, 0);
                    while (1)
                    {
                    };
                }
                else if (blades[ReceivedNUM - 1].hand_shake == OK_receive)
                {
                    // ��������
                    LED_Control(ReceivedNUM, Start_Mode, 0); //
                    if (Is_startUP[ReceivedNUM - 1] == false)
                    {
                        fan_OK_count++;
                        Is_startUP[ReceivedNUM - 1] = true;
                    }

                    osDelay(500); // ���ޱ�Ҫ��
                }
                if (fan_OK_count >= 1) // 5
                {
                    StartUP = true; // �������
                }
                DataReceived = false;
            }
/*-------------------------------- ������ ----------------------------------*/
#if 0
            for (uint8_t i = 0; i < 5; i++) // �ж��Ƿ����ID����
            {
                uint8_t ID_set_count = 0;
                if (blades[i].IDset != 0)
                {
                    ID_set_count++;
                }
                if (i == 4) // ѭ������ʱ�ж�
                {
                    if (ID_set_count == 0) // ȫ��û�н�������
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
                            random_num = (uint8_t)RNG_Get_RandomRange(1, 5); // ���������
                            if (knock[random_num - 1] == false)              // ���������Ѽ������Ҷ
                            {
                                break;
                            }
                        }
                    }
                    // CAN1_TX_blades(0x300 + random_num);        // ����ָ��
                    LED_Control(random_num, Inactive_Mode, 0); // ��Ӧ���ƣ�����δ����״̬

                    Uart3_callback(); // ���ڴ�ӡ

                    // ������ʱ��
                    last_tick = TIM6_tick;
                    time_started = true;
                }
                if ((TIM6_tick - last_tick) <= 2500 && ReceivedNUM == random_num) // ��ȷ����
                {
                    if (knock[random_num] = false)
                    {
                        fan_OK_count++;
                        time_started = false; // ���к�ֹͣ��ʱ
                    }
                    knock[random_num] = true;
                    // ���Ƹı�
                    LED_Control(random_num, Active_Mode, 0); // ��Ӧ���ƣ������Ѽ���״̬
                }
                else if (((TIM6_tick - last_tick) <= 2500 && ReceivedNUM != random_num) || (TIM6_tick - last_tick) >= 2500) // δ��ȷ����
                {
                    for (uint8_t i = 0; i < 5; i++)
                    {
                        knock[i] = false;
                        // ���Ƹı�
                        LED_Control(i, Close_Mode, 0); // ��Ӧ���
                        osDelay(100);
                    }
                    if ((TIM6_tick - last_tick) >= 2500)
                    {
                        time_started = false;
                    }
                }
                if (fan_OK_count == 5) // ȫ������
                {
                    // ����5��
                    for (uint8_t j = 0; j < 5; j++)
                    {
                        knock[j] = false;
                        for (uint8_t i = 0; i < 5; i++)
                        {
                            // ���Ƹı�
                            LED_Control(i, Active_Mode, 0);
                        }
                        osDelay(500);
                    }
                    // ���Ϩ��
                    for (uint8_t i = 0; i < 5; i++)
                    {
                        // ���Ƹı�
                        LED_Control(i, Close_Mode, 0); // ��Ӧ���
                        osDelay(1000);
                    }
                    fan_OK_count = 0;
                    osDelay(2000); // �ȴ�2s�����¿�ʼ
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
