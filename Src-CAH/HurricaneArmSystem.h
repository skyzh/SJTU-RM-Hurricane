//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANEARMSYSTEM_H
#define HURRICANE_HURRICANEARMSYSTEM_H

#include "CAHRR/src/PIDAccumulator.h"
#include "CAHRR/src/RampAccumulator.h"
#include "CAHRR/src/RotateAccumulator.h"
#include "CAHRR/src/DeltaAccumulator.h"
#include "CAHRR/src/AvgAccumulator.h"

#include "OI.h"
#include "hal.h"

const int ARM_BOTTOM_ID = 5;
const int ARM_BASE_ID = 7;

class ArmBottom;
class ArmTop;
class ArmBase;

class HurricaneArmSystem {
private:
    double position;
    ArmBottom* arm_bottom;
    ArmTop* arm_top;
    ArmBase* arm_base;

    // TODO: check if this flag is needed
    bool data_available_bottom, data_available_base;
public:

    bool disabled = true; // todo: remove

    HurricaneArmSystem();

    bool initialize();

    bool update();

    bool destroy();

    bool setPosition(double position);

    void data();

    void claw_trigger(uint32_t adc_value);
};

#endif //HURRICANE_HURRICANEARMSYSTEM_H
