//
// Created by Alex Chi on 2018/11/24.
//

#include "ArmTask.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneClawSystem.h"
#include "CAHRR/src/utils.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneChassisSystem.h"

ArmTask::ArmTask() : Task(), cur_position(0.5), servo_cur_position(0) {
}

bool ArmTask::initialize() {
    oi->debugSystem->info("TSK", "Enter arm drive");
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = false;
    oi->clawSystem->disabled = false;
    return true;
}

bool ArmTask::destroy() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = true;
    oi->clawSystem->disabled = false;
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
    if (oi->remoteSystem->getButton(CTR_SW1) == 1) {
        double delta = oi->remoteSystem->getAxis(CTR_CH1) / 660.0;
        servo_cur_position = clamp<double>(servo_cur_position + delta, 0, 100.0);
        oi->clawSystem->setPosition(map_range<double>(servo_cur_position, 0, 100, 0.0, 1.0),
                                    map_range<double>(oi->remoteSystem->getAxis(CTR_CH2), -AXIS_MAX, AXIS_MAX,
                                                      -1.0, 1.0)
        );
    } else {
        oi->armSystem->setPosition(
                map_range<double>(oi->remoteSystem->getAxis(CTR_CH2), -AXIS_MAX, AXIS_MAX, 0.3, 0.7));
    }


    return true;
}
