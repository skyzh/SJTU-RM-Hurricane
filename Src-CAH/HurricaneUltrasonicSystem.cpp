//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"


HurricaneUltrasonicSystem::HurricaneUltrasonicSystem(TIM_HandleTypeDef *tim,
                                                     uint16_t channel_trig,
                                                     uint16_t channel_echo) :
        tim(tim), channel_trig(channel_trig), channel_echo(channel_echo) {
    this->accumulator.reset();
    this->lst_time = 0;
}

bool HurricaneUltrasonicSystem::trigger(uint32_t cnt) {
    oi->debugSystem->info("UTS", std::to_string(cnt));
    this->accumulator.data(cnt);
    return true;
}

bool HurricaneUltrasonicSystem::initialize() {
    oi->debugSystem->info("UTS", "ultrasonic system initialize");
    HAL_TIM_IC_Start_IT(&htim8, channel_echo);
    HAL_TIM_PWM_Start(&htim8, channel_trig);
    oi->debugSystem->info("UTS", "  ... complete");
    return true;
}

bool HurricaneUltrasonicSystem::destroy() {
    return true;
}

double HurricaneUltrasonicSystem::distance() {
    return this->lst_time / 1000.0  / 1000.0 * 340.0 * 100.0; // unit: CM
}

bool HurricaneUltrasonicSystem::update() {
    this->lst_time = this->accumulator.sum();
    return true;
}

uint32_t HurricaneUltrasonicSystem::time() {
    return this->lst_time;
}

extern "C" void Hurricane_HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM8) {
        static bool TIM8_RISING = true;
        if (TIM8_RISING) {
            __HAL_TIM_SetCounter(&htim8, 0);
            TIM8_RISING = false;
        } else {
            TIM8_RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&htim8, oi->usSystemChassis->channel_echo);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(delta_time);
        }

    }
}
