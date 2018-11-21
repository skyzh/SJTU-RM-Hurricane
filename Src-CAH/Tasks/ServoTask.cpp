//
// Created by Alex Chi on 2018/11/19.
//

#include "ServoTask.h"
#include "hal.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCollectorSystem.h"
#include "CAHRR/src/utils.h"

const int SERVO_MIN_PERIOD = 20000 / 20 * 0.5;
const int SERVO_MAX_PERIOD = 20000 / 20 * 2.5;

ServoTask::ServoTask() : Task() {

}

bool ServoTask::initialize() {
    oi->collectorSystem->setPosition(0);
    return true;
}

bool ServoTask::destroy() {
    return true;
}

bool ServoTask::isEnd() {
    return false;
}

bool ServoTask::update() {
    oi->collectorSystem->setPosition(map_range<double>(oi->remoteSystem->getAxis(1), -660, 660, 0.0, 1.0));
    return true;
}
