//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"

#define UTS1_TIM htim8
#define UTS2_TIM htim8
#define UTS1_CHANNEL TIM_CHANNEL_3
#define UTS2_CHANNEL TIM_CHANNEL_4
#define UTS1_TIM_INSTANCE TIM8
#define UTS2_TIM_INSTANCE TIM8

HurricaneUltrasonicSystem::HurricaneUltrasonicSystem() : trig_current(0), avg_cnt(0) {
    for (int i = 0; i < UTS_MAX_ID; i++) {
        this->accumulator[i].reset();
    }
    memset(this->lst_time, 0, sizeof this->lst_time);
}

bool HurricaneUltrasonicSystem::trigger(TIM_TypeDef *instance, uint32_t channel, uint32_t cnt) {
    if (instance == UTS1_TIM_INSTANCE && channel == UTS1_CHANNEL) {
        this->accumulator[0].data(cnt);
    } else if (instance == UTS2_TIM_INSTANCE && channel == UTS2_CHANNEL) {
        this->accumulator[1].data(cnt);
    }
    avg_cnt = (avg_cnt + 1) % 5;
    if (avg_cnt == 0) {
        this->trig_current = (this->trig_current + 1) % UTS_MAX_ID;
    }
    return true;
}

bool HurricaneUltrasonicSystem::initialize() {
    oi->debugSystem->info("UTS", "ultrasonic system initialize");
    HAL_TIM_IC_Start_IT(&UTS1_TIM, UTS1_CHANNEL);
    HAL_TIM_IC_Start_IT(&UTS2_TIM, UTS2_CHANNEL);
    this->trig_current = 0;
    oi->debugSystem->info("UTS", "  ... complete");
    return true;
}

bool HurricaneUltrasonicSystem::destroy() {
    HAL_TIM_IC_Stop_IT(&UTS1_TIM, UTS1_CHANNEL);
    HAL_TIM_IC_Stop_IT(&UTS2_TIM, UTS2_CHANNEL);
    return true;
}

double HurricaneUltrasonicSystem::distance(int id) {
    return this->lst_time[id] / 1000.0 / 1000.0 * 340.0 * 100.0 / 2.0; // unit: CM
}

bool HurricaneUltrasonicSystem::update() {
    this->do_trig(0);
    for (int i = 0; i < UTS_MAX_ID; i++) {
        if (this->accumulator[i].n != 0) this->lst_time[i] = this->accumulator[i].sum();
    }
    return true;
}

uint32_t HurricaneUltrasonicSystem::time(int id) {
    return this->lst_time[id];
}

bool HurricaneUltrasonicSystem::available(int id) {
    return true;
}

bool HurricaneUltrasonicSystem::do_trig(int id) {
    switch (id) {
        case 0:
            HAL_GPIO_WritePin(US1_TRIG_GPIO_Port, US1_TRIG_Pin, GPIO_PIN_SET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(US1_TRIG_GPIO_Port, US1_TRIG_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(US2_TRIG_GPIO_Port, US2_TRIG_Pin, GPIO_PIN_SET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(US2_TRIG_GPIO_Port, US2_TRIG_Pin, GPIO_PIN_RESET);
            break;
    }
    return false;
}

extern "C" void Hurricane_HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == UTS1_TIM_INSTANCE) {
        static bool RISING = true;
        if (RISING) {
                    __HAL_TIM_SetCounter(&htim8, 0);
            RISING = false;
        } else {
            RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&htim8, htim->Channel);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(htim->Instance, htim->Channel, delta_time);
        }
        oi->debugSystem->toggle_led(2);
    }
    if (htim->Instance == UTS2_TIM_INSTANCE && htim->Channel == UTS2_CHANNEL) {
        static bool RISING = true;
        if (RISING) {
                    __HAL_TIM_SetCounter(&htim8, 0);
            RISING = false;
        } else {
            RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&htim8, htim->Channel);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(htim->Instance, htim->Channel, delta_time);
        }
        oi->debugSystem->toggle_led(4);
    }
}
