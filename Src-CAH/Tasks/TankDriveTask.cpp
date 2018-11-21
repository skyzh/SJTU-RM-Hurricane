//
// Created by Alex Chi on 2018/11/17.
//

#include "TankDriveTask.h"
#include "../OI.h"
#include "HurricaneChassisSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"

const double AXIS_MAX = 660;

TankDriveTask::TankDriveTask() : Task() {

}

bool TankDriveTask::initialize() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = false;
    return true;
}

bool TankDriveTask::destroy() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = true;
    return true;
}

bool TankDriveTask::isEnd() {
    return false;
}

bool TankDriveTask::update() {
    oi->chassisSystem->setSpeed(oi->remoteSystem->getAxis(1) / AXIS_MAX, oi->remoteSystem->getAxis(0) / AXIS_MAX,
                                oi->remoteSystem->getAxis(2) / AXIS_MAX);
    return true;
}
