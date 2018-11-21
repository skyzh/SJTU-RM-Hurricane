//
// Created by Alex Chi on 2018/11/13.
//

#ifndef HURRICANE_HURRICANEULTRASONICSYSTEM_H
#define HURRICANE_HURRICANEULTRASONICSYSTEM_H

#include "hal.h"
#include "CAHRR/src/UltrasonicSystem.h"
#include "CAHRR/src/AvgAccumulator.h"

class HurricaneUltrasonicSystem {
private:
    TIM_HandleTypeDef *tim;
    AvgAccumulator<uint32_t> accumulator;
    uint32_t lst_time;
    bool data_available;
public:
    const uint16_t channel_trig, channel_echo;

    HurricaneUltrasonicSystem(TIM_HandleTypeDef *tim, uint16_t channel_trig, uint16_t channel_echo);

    bool trigger(uint32_t cnt);

    bool update();

    bool initialize();

    bool destroy();

    bool available();

    double distance();

    uint32_t time();
};


#endif //HURRICANE_HURRICANEULTRASONICSYSTEM_H
