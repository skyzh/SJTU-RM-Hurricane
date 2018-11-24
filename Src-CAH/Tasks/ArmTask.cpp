//
// Created by Alex Chi on 2018/11/24.
//

#include "ArmTask.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneDebugSystem.h"
#include "CAHRR/src/utils.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneChassisSystem.h"

ArmTask::ArmTask() : Task(), cur_position(0.5) {
}

bool ArmTask::initialize() {
    oi->debugSystem->info("TSK", "Enter arm drive");
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = false;
    return true;
}

bool ArmTask::destroy() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = true;
    oi->debugSystem->info("TSK", "Exit arm drive");
    return true;
}

bool ArmTask::isEnd() {
    return false;
}

bool ArmTask::update() {
    const double AXIS_MAX = 660;
    oi->chassisSystem->setSpeed(
            -oi->remoteSystem->getAxis(CTR_CH4) / AXIS_MAX / 5.0,
            -oi->remoteSystem->getAxis(CTR_CH3) / AXIS_MAX / 5.0,
            0);
    oi->armSystem->setPosition(map_range<double>(oi->remoteSystem->getAxis(CTR_CH2), -AXIS_MAX, AXIS_MAX, 0.3, 0.7));
    return true;
}
