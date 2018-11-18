//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANEARMSYSTEM_H
#define HURRICANE_HURRICANEARMSYSTEM_H

#include "CAHRR/src/PIDAccumulator.h"
#include "CAHRR/src/RampAccumulator.h"
#include "CAHRR/src/RotateAccumulator.h"
#include "OI.h"

const int ARM_BOTTOM_ID = 5;

class HurricaneArmSystem {
private:
    PIDDisplacementAccumulator *pid_bottom;
    RampAccumulator <double> *ramp_bottom;
    RotateAccumulator *accumulator_bottom;
    double position;
    // TODO: check if this flag is needed
    bool data_available_bottom;
public:
    HurricaneArmSystem();

    bool initialize();

    bool update();

    bool destroy();

    bool setPosition(double position);

    void data();
};

#endif //HURRICANE_HURRICANEARMSYSTEM_H
