#include "key.h"

bool key_Press[11];
#ifdef __cplusplus
extern "C"
{
#endif

    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
        switch (GPIO_Pin)
        {
        case GPIO_PIN_0:
            key_Press[6] = true;
            break;
        case GPIO_PIN_1:
            key_Press[7] = true;
            break;
        case GPIO_PIN_2:
            key_Press[8] = true;
            break;
        case GPIO_PIN_3:
            key_Press[1] = true;
            break;
        case GPIO_PIN_4:
            key_Press[2] = true;
            break;
        case GPIO_PIN_5:
            key_Press[3] = true;
            break;
        case GPIO_PIN_6:
            key_Press[4] = true;
            break;
        case GPIO_PIN_7:
            key_Press[5] = true;
            break;
        case GPIO_PIN_10:
            key_Press[9] = true;
            break;
        case GPIO_PIN_11:
            key_Press[10] = true;
            break;
        }
    }

#ifdef __cplusplus
}
#endif