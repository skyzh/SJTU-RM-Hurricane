//
// Created by Alex Chi on 2018/11/12.
//

#include "CAHRR/src/RotateAccumulator.h"
#include "CAHRR/src/PIDAccumulator.h"
#include "MainTask.h"
#include "OI.h"
#include "HurricaneIMUSystem.h"
#include "HurricaneCANSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneUltrasonicSystem.h"
#include "HurricaneChassisSystem.h"

const double AXIS_MAX = 660;

MainTask::MainTask() : Task() {

}

bool MainTask::initialize() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    return true;
}

bool MainTask::destroy() {
    return false;
}

bool MainTask::isEnd() {
    return false;
}

char buf[1000];

bool MainTask::update() {
    oi->chassisSystem->setSpeed(oi->remoteSystem->getAxis(1) / AXIS_MAX, oi->remoteSystem->getAxis(0) / AXIS_MAX, oi->remoteSystem->getAxis(2) / AXIS_MAX);
    return true;
}

MainTask::~MainTask() {

}

Task *mainTask() {
    return new MainTask;
}
