//
// Created by Alex Chi on 2018/11/17.
//

#include "ArmTask.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"
ArmTask::ArmTask() : Task() {

}

bool ArmTask::initialize() {
    oi->armSystem->setPosition(0);
    return true;
}

bool ArmTask::destroy() {
    oi->armSystem->setPosition(0);
    return true;
}

bool ArmTask::isEnd() {
    return false;
}

bool ArmTask::update() {
    oi->armSystem->setPosition((oi->remoteSystem->getAxis(0) / 660.0) * 15.0);
    return true;
}
