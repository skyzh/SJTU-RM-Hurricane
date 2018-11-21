//
// Created by Alex Chi on 2018/11/22.
//

#include "ClawTask.h"
#include "hal.h"
#include "CAHRR/src/utils.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"

ClawTask::ClawTask() : Task() {

}

bool ClawTask::initialize() {
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
    return true;
}

bool ClawTask::destroy() {
    HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_4);
    return true;
}

bool ClawTask::isEnd() {
    return false;
}

bool ClawTask::update() {
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, map_range<double>(oi->remoteSystem->getAxis(2), -660, 660, 0, 19999));
    return true;
}
