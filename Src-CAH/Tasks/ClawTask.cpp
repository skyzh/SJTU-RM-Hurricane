//
// Created by Alex Chi on 2018/11/24.
//

#include "ClawTask.h"
#include "OI.h"
#include "HurricaneClawSystem.h"
#include "HurricaneDebugSystem.h"

bool ClawOpenTask::initialize() {
    oi->debugSystem->info("TSK", "claw open");
    return true;
}

bool ClawOpenTask::destroy() {
    return true;
}

bool ClawOpenTask::update() {
    oi->clawSystem->setPosition(0.0);
    return true;
}

bool ClawCloseTask::initialize() {
    oi->debugSystem->info("TSK", "claw close");
    return true;
}

bool ClawCloseTask::destroy() {
    return true;
}

bool ClawCloseTask::update() {
    oi->clawSystem->setPosition(1.0);
    return true;
}
