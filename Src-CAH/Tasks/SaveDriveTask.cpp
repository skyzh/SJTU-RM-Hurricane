//
// Created by Alex Chi on 2018/11/24.
//

#include "SaveDriveTask.h"
#include "OI.h"
#include "HurricaneChassisSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneRemoteSystem.h"

SaveDriveTask::SaveDriveTask() : Task() {

}

bool SaveDriveTask::initialize() {
    oi->debugSystem->info("TSK", "Enter save drive");
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = false;
    HAL_GPIO_WritePin(MAGNET_GPIO_Port, MAGNET_Pin, GPIO_PIN_SET);
    return true;
}

bool SaveDriveTask::destroy() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    oi->chassisSystem->disabled = true;
    HAL_GPIO_WritePin(MAGNET_GPIO_Port, MAGNET_Pin, GPIO_PIN_RESET);
    oi->debugSystem->info("TSK", "Exit save drive");
    return true;
}

bool SaveDriveTask::isEnd() {
    return false;
}

bool SaveDriveTask::update() {
    const double AXIS_MAX = 660;
    oi->chassisSystem->setSpeed(
            oi->remoteSystem->getAxis(CTR_CH4) / AXIS_MAX / 3.0,
            oi->remoteSystem->getAxis(CTR_CH3) / AXIS_MAX / 3.0,
            oi->remoteSystem->getAxis(CTR_CH1) / AXIS_MAX / 3.0);
    return true;
}
