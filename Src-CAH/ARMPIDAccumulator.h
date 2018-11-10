//
// Created by Alex Chi on 2018/11/09.
//

#ifndef HURRICANE_ARMPIDACCUMULATOR_H
#define HURRICANE_ARMPIDACCUMULATOR_H

#include "hal.h"
#include "arm_math.h"

class ARMPIDAccumulator {
private:
    arm_pid_instance_q31 instance;
public:
    ARMPIDAccumulator(int32_t p, int32_t i, int32_t d);

    bool reset();

    int32_t calc(int32_t in);
};


#endif //HURRICANE_ARMPIDACCUMULATOR_H
