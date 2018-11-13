//
// Created by Alex Chi on 2018/11/13.
//

#include "hal.h"
#include "OI.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == US_INPUT_Pin) {
        if (oi && oi->usSystemChassis) oi->usSystemChassis->trigger();
    }
}
