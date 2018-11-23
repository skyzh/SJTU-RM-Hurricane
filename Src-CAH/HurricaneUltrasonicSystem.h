//
// Created by Alex Chi on 2018/11/13.
//

#ifndef HURRICANE_HURRICANEULTRASONICSYSTEM_H
#define HURRICANE_HURRICANEULTRASONICSYSTEM_H

#include "hal.h"
#include "CAHRR/src/UltrasonicSystem.h"
#include "CAHRR/src/AvgAccumulator.h"

const int UTS_MAX_ID = 4;

class HurricaneUltrasonicSystem {
private:
    AvgAccumulator<uint32_t> accumulator[UTS_MAX_ID];
    uint32_t lst_time[UTS_MAX_ID];
    int trig_current;
    int avg_cnt;
    uint32_t current_channel;
    TIM_TypeDef* current_tim;
    int reset_cnt;
    bool do_trig(int id);
public:

    HurricaneUltrasonicSystem();

    bool trigger(TIM_TypeDef *instance, uint32_t channel, uint32_t cnt);

    bool update();

    bool initialize();

    bool destroy();

    bool available(int id);

    double distance(int id);

    uint32_t time(int id);
};


#endif //HURRICANE_HURRICANEULTRASONICSYSTEM_H
