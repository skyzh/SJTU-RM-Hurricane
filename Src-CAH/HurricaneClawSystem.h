//
// Created by Alex Chi on 2018/11/22.
//

#ifndef HURRICANE_HURRICANECLAWSYSTEM_H
#define HURRICANE_HURRICANECLAWSYSTEM_H

#include "hal.h"
#include "CAHRR/src/AvgAccumulator.h"
#include "CAHRR/src/PIDAccumulator.h"
#include "CAHRR/src/DeltaAccumulator.h"

class HurricaneClawSystem {
private:
    double target_position;
    AvgAccumulator<int> avg;
    DeltaAccumulator<int> spd;
    PIDRateAccumulator rate;
    PIDDisplacementAccumulator pos;

public:
    bool disabled = false;

    HurricaneClawSystem();

    bool initialize();

    bool update();

    bool destroy();

    bool setPosition(double position);

    void trigger(uint32_t adc_value);
};

#endif //HURRICANE_HURRICANECLAWSYSTEM_H
