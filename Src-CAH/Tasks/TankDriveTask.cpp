//
// Created by Alex Chi on 2018/11/17.
//

#include "TankDriveTask.h"
#include "../OI.h"
#include "HurricaneChassisSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"

TankDriveTask::TankDriveTask() : Task() {

}

bool TankDriveTask::initialize() {
    oi->debugSystem->info("TSK", "Enter tank drive");
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = false;
    return true;
}

bool TankDriveTask::destroy() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = true;
    oi->debugSystem->info("TSK", "Exit tank drive");
    return true;
}

bool TankDriveTask::isEnd() {
    return false;
}

bool TankDriveTask::update() {
    const double AXIS_MAX = 660;
    oi->chassisSystem->setSpeed(
            -oi->remoteSystem->getAxis(CTR_CH4) / AXIS_MAX,
            -oi->remoteSystem->getAxis(CTR_CH3) / AXIS_MAX,
            oi->remoteSystem->getAxis(CTR_CH1) / AXIS_MAX);
    return true;
}
