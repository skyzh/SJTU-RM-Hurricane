//
// Created by Alex Chi on 2018/11/13.
//

#include "hal.h"
#include "OI.h"
#include "HurricaneUltrasonicSystem.h"
#include "HurricaneDebugSystem.h"

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    oi->debugSystem->info("UTS", "triggered");
    if (GPIO_Pin == US_ECHO_Pin) {
        if (oi && oi->usSystemChassis) oi->usSystemChassis->trigger();
    }
}
