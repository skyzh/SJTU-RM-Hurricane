//
// Created by Alex Chi on 2018/11/22.
//

#include "HurricaneClawSystem.h"
#include "HurricaneDebugSystem.h"
#include "OI.h"
#include "CAHRR/src/utils.h"
#include "HurricaneRemoteSystem.h"

#define CLAW_ADC hadc1
#define CLAW_TIM htim5
#define CLAW_CHANNEL TIM_CHANNEL_4

const double CLAW_MAX_OUTPUT = 12000;
const double CLAW_SPD_LIMIT = 10;
const int CLAW_MIN_POS = 2090;
const int CLAW_MAX_POS = 3500;

HurricaneClawSystem::HurricaneClawSystem() : target_position(0) {

    this->avg.reset();
    this->spd.reset();
    this->pos.set_pid(1.0, 0.0, 0.5);
    this->pos.set_output(-CLAW_SPD_LIMIT, CLAW_SPD_LIMIT);
    this->rate.set_pid(10.0, 0.0, 5.0);
    this->rate.set_output(-CLAW_MAX_OUTPUT, CLAW_MAX_OUTPUT);
}

bool HurricaneClawSystem::initialize() {
    OK(oi->debugSystem->info("CLA", "claw system initialize"));
    HAL_ADC_Start_IT(&CLAW_ADC);
    HAL_TIM_PWM_Start(&CLAW_TIM, CLAW_CHANNEL);
    HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
    this->avg.reset();
    this->spd.reset();
    this->rate.reset();
    this->pos.reset();
    OK(oi->debugSystem->info("CLA", "  ... complete"));
    return true;
}

bool HurricaneClawSystem::update() {
    double current_position = this->avg.sum();
    double target_speed = clamp(this->pos.calc(this->target_position - current_position), -CLAW_SPD_LIMIT, CLAW_SPD_LIMIT);
    double current_speed = this->spd.delta(current_position);
    double target_output = clamp(this->rate.calc(target_speed - current_speed), -CLAW_MAX_OUTPUT, CLAW_MAX_OUTPUT);

    if (disabled) {
        HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&CLAW_TIM, CLAW_CHANNEL, 0);
    } else {
        if (target_output < 0) {
            HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_SET);
            __HAL_TIM_SET_COMPARE(&CLAW_TIM, CLAW_CHANNEL, CLAW_MAX_OUTPUT - abs(target_output));
        } else {
            HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SET_COMPARE(&CLAW_TIM, CLAW_CHANNEL, target_output);
        }
    }

    HDEBUG_BEGIN(50)
    static char buf[100];
    sprintf(buf, "tspd %.2f cspd %.2f, tpos %.2f, cpos %.2f, out %.2f", target_speed, current_speed, target_position, current_position, target_output);
    oi->debugSystem->info("CLA", buf);
    HDEBUG_END()
    return true;
}

bool HurricaneClawSystem::destroy() {
    HAL_GPIO_WritePin(CLAW_OUT_GPIO_Port, CLAW_OUT_Pin, GPIO_PIN_RESET);
    HAL_ADC_Stop_IT(&CLAW_ADC);
    HAL_TIM_PWM_Stop(&CLAW_TIM, CLAW_CHANNEL);
    return true;
}

bool HurricaneClawSystem::setPosition(double position) {
    this->target_position = map_range<double>(1 - position, 0.0, 1.0, CLAW_MIN_POS, CLAW_MAX_POS);
    return true;
}

void HurricaneClawSystem::trigger(uint32_t adc_value) {
    this->avg.data(adc_value);
}

extern "C" void Hurricane_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc == &CLAW_ADC) {
        HSYS(clawSystem) oi->clawSystem->trigger(HAL_ADC_GetValue(&CLAW_ADC));
    }
}
