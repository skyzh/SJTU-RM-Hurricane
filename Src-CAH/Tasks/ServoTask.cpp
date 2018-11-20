//
// Created by Alex Chi on 2018/11/19.
//

#include "ServoTask.h"
#include "hal.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"

const int SERVO_MIN_PERIOD = 20000 / 20 * 0.5;
const int SERVO_MAX_PERIOD = 20000 / 20 * 2.5;

inline double map_range(double in, double in_min, double in_max, double out_min, double out_max) {
    double out_range = out_max - out_min;
    double in_range = in_max - in_min;
    return (in - in_min) / in_range * out_range + out_min;
}

ServoTask::ServoTask() : Task() {

}

bool ServoTask::initialize() {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    return true;
}

bool ServoTask::destroy() {
    return true;
}

bool ServoTask::isEnd() {
    return false;
}

bool ServoTask::update() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, map_range(oi->remoteSystem->getAxis(0), -660, 660, SERVO_MIN_PERIOD, SERVO_MAX_PERIOD));
    return true;
}
