//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"

#define UTS1_TIM htim8
#define UTS1_TIM_INSTANCE TIM8
#define UTS1_ECHO_CHANNEL TIM_CHANNEL_3
#define UTS1_TRIG_CHANNEL TIM_CHANNEL_1

#define UTS2_TIM htim4
#define UTS2_ECHO_CHANNEL TIM_CHANNEL_3
#define UTS2_TRIG_CHANNEL TIM_CHANNEL_1
#define UTS2_TIM_INSTANCE TIM4

#define UTS3_TIM htim8
#define UTS3_ECHO_CHANNEL TIM_CHANNEL_4
#define UTS3_TRIG_CHANNEL TIM_CHANNEL_2
#define UTS3_TIM_INSTANCE TIM8

#define UTS4_TIM htim4
#define UTS4_ECHO_CHANNEL TIM_CHANNEL_4
#define UTS4_TRIG_CHANNEL TIM_CHANNEL_2
#define UTS4_TIM_INSTANCE TIM4

HurricaneUltrasonicSystem::HurricaneUltrasonicSystem() :
        trig_current(0), avg_cnt(0), reset_cnt(0) {
    for (int i = 0; i < UTS_MAX_ID; i++) {
        this->accumulator[i].reset();
    }
    this->current_tim = nullptr;
    this->current_channel = 0;
    memset(this->lst_time, 0, sizeof this->lst_time);
}

bool HurricaneUltrasonicSystem::trigger(TIM_TypeDef *instance, uint32_t channel, uint32_t cnt) {
    if (instance == this->current_tim && channel == this->current_channel) {
        this->accumulator[this->trig_current].data(cnt);
        avg_cnt = (avg_cnt + 1) % 5;
        if (avg_cnt == 0) {
            this->trig_current = (this->trig_current + 1) % UTS_MAX_ID;
            this->do_trig(this->trig_current);
        }
        return true;
    }
    return false;
}

bool HurricaneUltrasonicSystem::initialize() {
    oi->debugSystem->info("UTS", "ultrasonic system initialize");
    HAL_TIM_IC_Start_IT(&UTS1_TIM, UTS1_ECHO_CHANNEL);
    HAL_TIM_IC_Start_IT(&UTS2_TIM, UTS2_ECHO_CHANNEL);
    HAL_TIM_IC_Start_IT(&UTS3_TIM, UTS3_ECHO_CHANNEL);
    HAL_TIM_IC_Start_IT(&UTS4_TIM, UTS4_ECHO_CHANNEL);
    this->trig_current = 0;
    this->do_trig(this->trig_current);
    oi->debugSystem->info("UTS", "  ... complete");
    return true;
}

bool HurricaneUltrasonicSystem::destroy() {
    HAL_TIM_IC_Stop_IT(&UTS1_TIM, UTS1_ECHO_CHANNEL);
    HAL_TIM_IC_Stop_IT(&UTS2_TIM, UTS2_ECHO_CHANNEL);
    HAL_TIM_IC_Stop_IT(&UTS3_TIM, UTS3_ECHO_CHANNEL);
    HAL_TIM_IC_Stop_IT(&UTS4_TIM, UTS4_ECHO_CHANNEL);
    return true;
}

double HurricaneUltrasonicSystem::distance(int id) {
    return this->lst_time[id] / 1000.0 / 1000.0 * 340.0 * 100.0 / 2.0; // unit: CM
}

bool HurricaneUltrasonicSystem::update() {
    for (int i = 0; i < UTS_MAX_ID; i++) {
        if (this->accumulator[i].n != 0) this->lst_time[i] = this->accumulator[i].sum();
    }
    this->reset_cnt += 1;
    if (this->reset_cnt == 10) {
        this->reset_cnt = 0;
        this->trig_current = (this->trig_current + 1) % UTS_MAX_ID;
        this->do_trig(this->trig_current);
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
    HAL_TIM_PWM_Stop(&UTS1_TIM, UTS1_TRIG_CHANNEL);
    HAL_TIM_PWM_Stop(&UTS2_TIM, UTS2_TRIG_CHANNEL);
    HAL_TIM_PWM_Stop(&UTS3_TIM, UTS3_TRIG_CHANNEL);
    HAL_TIM_PWM_Stop(&UTS4_TIM, UTS4_TRIG_CHANNEL);

    switch (id) {
        case 0:
            this->current_channel = UTS1_ECHO_CHANNEL;
            this->current_tim = UTS1_TIM_INSTANCE;
            HAL_TIM_PWM_Start(&UTS1_TIM, UTS1_TRIG_CHANNEL);
            break;
        case 1:
            this->current_channel = UTS2_ECHO_CHANNEL;
            this->current_tim = UTS2_TIM_INSTANCE;
            HAL_TIM_PWM_Start(&UTS2_TIM, UTS2_TRIG_CHANNEL);
            break;
        case 2:
            this->current_channel = UTS3_ECHO_CHANNEL;
            this->current_tim = UTS3_TIM_INSTANCE;
            HAL_TIM_PWM_Start(&UTS3_TIM, UTS3_TRIG_CHANNEL);
            break;
        case 3:
            this->current_channel = UTS4_ECHO_CHANNEL;
            this->current_tim = UTS4_TIM_INSTANCE;
            HAL_TIM_PWM_Start(&UTS4_TIM, UTS4_TRIG_CHANNEL);
            break;
    }
    this->reset_cnt = 0;
    return true;
}

extern "C" void Hurricane_HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == UTS1_TIM_INSTANCE && htim->Channel == UTS1_ECHO_CHANNEL - 4) {
        static bool RISING = true;
        if (RISING) {
                    __HAL_TIM_SetCounter(&UTS1_TIM, 0);
            RISING = false;
        } else {
            RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&UTS1_TIM, UTS1_ECHO_CHANNEL);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(UTS1_TIM_INSTANCE, UTS1_ECHO_CHANNEL, delta_time);
        }
    }
    if (htim->Instance == UTS2_TIM_INSTANCE && htim->Channel == UTS2_ECHO_CHANNEL - 4) {
        static bool RISING = true;
        if (RISING) {
                    __HAL_TIM_SetCounter(&UTS2_TIM, 0);
            RISING = false;
        } else {
            RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&UTS2_TIM, UTS2_ECHO_CHANNEL);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(UTS2_TIM_INSTANCE, UTS2_ECHO_CHANNEL, delta_time);
        }
    }
    if (htim->Instance == UTS3_TIM_INSTANCE && htim->Channel == UTS3_ECHO_CHANNEL - 4) {
        static bool RISING = true;
        if (RISING) {
                    __HAL_TIM_SetCounter(&UTS3_TIM, 0);
            RISING = false;
        } else {
            RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&UTS3_TIM, UTS3_ECHO_CHANNEL);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(UTS3_TIM_INSTANCE, UTS3_ECHO_CHANNEL, delta_time);
        }
    }
    if (htim->Instance == UTS4_TIM_INSTANCE && htim->Channel == UTS4_ECHO_CHANNEL - 4) {
        static bool RISING = true;
        if (RISING) {
                    __HAL_TIM_SetCounter(&UTS4_TIM, 0);
            RISING = false;
        } else {
            RISING = true;
            uint32_t delta_time = __HAL_TIM_GetCompare(&UTS4_TIM, UTS4_ECHO_CHANNEL);
            HSYS(usSystemChassis) oi->usSystemChassis->trigger(UTS4_TIM_INSTANCE, UTS4_ECHO_CHANNEL, delta_time);
        }
    }
}
