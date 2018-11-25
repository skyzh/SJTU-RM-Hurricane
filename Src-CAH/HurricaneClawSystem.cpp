//
// Created by Alex Chi on 2018/11/22.
//

#include "HurricaneClawSystem.h"
#include "HurricaneDebugSystem.h"
#include "OI.h"
#include "CAHRR/src/utils.h"
#include "HurricaneRemoteSystem.h"

#define CLAW_ADC hadc1

#define CLAW_BUFF_TIM htim8
#define CLAW_BUFF_CHANNEL TIM_CHANNEL_1

const double CLAW_MAX_OUTPUT = 20000;
const int CLAW_MIN_POS = 3000;
const int CLAW_MAX_POS = 8000;

HurricaneClawSystem::HurricaneClawSystem() : target_position(0) {
}

bool HurricaneClawSystem::initialize() {
    OK(oi->debugSystem->info("CLA", "claw system initialize"));
    HAL_TIM_PWM_Start(&CLAW_BUFF_TIM, CLAW_OUT_Pin);
    HAL_GPIO_WritePin(CLAW_DEOUT_GPIO_Port, CLAW_DEOUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
    OK(oi->debugSystem->info("CLA", "  ... complete"));
    return true;
}

bool HurricaneClawSystem::update() {
    double target_output = this->target_height;
    if (disabled) {
        HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&CLAW_BUFF_TIM, CLAW_OUT_Pin, 0);
    } else {
        if (target_output < 0) {
            HAL_GPIO_WritePin(CLAW_DEOUT_GPIO_Port, CLAW_DEOUT_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
        } else if (target_output > 0) {
            HAL_GPIO_WritePin(CLAW_DEOUT_GPIO_Port, CLAW_DEOUT_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(CLAW_DEOUT_GPIO_Port, CLAW_DEOUT_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
        }
        __HAL_TIM_SET_COMPARE(&CLAW_BUFF_TIM, CLAW_BUFF_CHANNEL, this->target_position);
    }
    return true;
}

bool HurricaneClawSystem::destroy() {
    HAL_GPIO_WritePin(CLAW_DEOUT_GPIO_Port, CLAW_DEOUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&CLAW_BUFF_TIM, CLAW_OUT_Pin);
    return true;
}

bool HurricaneClawSystem::setPosition(double position, double height) {
    this->target_position = map_range<double>(position, 0.0, 1.0, CLAW_MIN_POS, CLAW_MAX_POS);
    this->target_height = map_range<double>(height, -1.0, 1.0, -CLAW_MAX_OUTPUT, CLAW_MAX_OUTPUT);;
    return true;
}


extern "C" void Hurricane_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
}
