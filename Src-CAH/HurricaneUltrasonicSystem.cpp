//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"


const int UTS_STATUS_PENDING = 0;
const int UTS_STATUS_SENT = 1;
const int UTS_STATUS_WAIT = 2;
const int UTS_STATUS_ECHO = 3;

HurricaneUltrasonicSystem::HurricaneUltrasonicSystem(TIM_HandleTypeDef *tim,
                                                     uint16_t channel_trig,
                                                     uint16_t channel_echo) :
        tim(tim), channel_trig(channel_trig), channel_echo(channel_echo) {
    this->accumulator.reset();
    this->rising = true;
    this->lst_time = 0;
}

bool HurricaneUltrasonicSystem::trigger() {
    if (this->rising) {
        __HAL_TIM_SetCounter(&htim8, 0);
        this->rising = false;
    } else {
        this->rising = true;
        this->accumulator.data(__HAL_TIM_GetCompare(&htim8, channel_echo));
        __HAL_TIM_SetCounter(&htim8, 0);
    }
    return true;
}

bool HurricaneUltrasonicSystem::initialize() {
    oi->debugSystem->info("UTS", "ultrasonic system initialize");
    this->rising = true;
    HAL_TIM_IC_Start_IT(&htim8, channel_echo);
    HAL_TIM_PWM_Start(&htim8, channel_trig);
    oi->debugSystem->info("UTS", "  ... complete");
    return true;
}

bool HurricaneUltrasonicSystem::destroy() {
    return true;
}

double HurricaneUltrasonicSystem::distance() {
    // TODO: calculate exact distance
    return 0;
}

bool HurricaneUltrasonicSystem::update() {
    this->lst_time = this->accumulator.sum();
    return true;
}

uint32_t HurricaneUltrasonicSystem::time() {
    return this->lst_time;
}

extern "C" void Hurricane_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM8) {
        if (oi && oi->usSystemChassis) oi->usSystemChassis->trigger();
    }
}
