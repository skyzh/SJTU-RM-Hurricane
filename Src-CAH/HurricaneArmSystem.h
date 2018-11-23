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
const int ARM_TOP_ID = 6;
const int ARM_BASE_ID = 7;

#define HURRICANE_ARM_TOP_DISABLE

class ArmBottom;
class ArmTop;
class ArmBase;

class HurricaneArmSystem {
private:
    double position;
    ArmBottom* arm_bottom;
#ifndef HURRICANE_ARM_TOP_DISABLE
    ArmTop* arm_top;
#endif
    ArmBase* arm_base;

    // TODO: check if this flag is needed
    bool data_available_bottom, data_available_base, data_available_top;
public:

    bool disabled = true; // todo: remove

    HurricaneArmSystem();

    bool initialize();

    bool update();

    bool destroy();

    bool setPosition(double position);

    void data();
};

#endif //HURRICANE_HURRICANEARMSYSTEM_H
