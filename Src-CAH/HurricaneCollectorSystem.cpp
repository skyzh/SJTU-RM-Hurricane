//
// Created by Alex Chi on 2018/11/21.
//

#include "HurricaneCollectorSystem.h"
#include "hal.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "CAHRR/src/utils.h"

const int SERVO_MIN_PERIOD = 20000 / 20 * 0.4;
const int SERVO_MAX_PERIOD = 20000 / 20 * 1.95;

#define COLLECTOR_TIM htim2
#define COLLECTOR_TIM_CH TIM_CHANNEL_3

HurricaneCollectorSystem::HurricaneCollectorSystem() : position(1.0) {
}

bool HurricaneCollectorSystem::initialize() {
    OK(oi->debugSystem->info("CLT", "collector system initialize"));
    HAL_TIM_PWM_Start(&COLLECTOR_TIM, COLLECTOR_TIM_CH);
    OK(oi->debugSystem->info("CLT", "  ... complete"));
    return true;
}

bool HurricaneCollectorSystem::update() {
    __HAL_TIM_SET_COMPARE(&COLLECTOR_TIM, COLLECTOR_TIM_CH, map_range<double>(this->position, 0.0, 1.0, SERVO_MIN_PERIOD, SERVO_MAX_PERIOD));
    return true;
}

bool HurricaneCollectorSystem::destroy() {
    HAL_TIM_PWM_Stop(&COLLECTOR_TIM, COLLECTOR_TIM_CH);
    return true;
}

bool HurricaneCollectorSystem::setPosition(double position) {
    this->position = position;
    return true;
}
