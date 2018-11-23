//
// Created by Alex Chi on 2018/11/24.
//

#include "ServoTask.h"
#include "OI.h"
#include "HurricaneCollectorSystem.h"
#include "HurricaneDebugSystem.h"

bool CollectorOpenTask::initialize() {
    oi->debugSystem->info("TSK", "collector open");
    return true;
}

bool CollectorOpenTask::destroy() {
    return true;
}

bool CollectorOpenTask::update() {
    oi->collectorSystem->setPosition(0.0);
    return true;
}

bool CollectorCloseTask::initialize() {
    oi->debugSystem->info("TSK", "collector close");
    return true;
}

bool CollectorCloseTask::destroy() {
    return true;
}

bool CollectorCloseTask::update() {
    oi->collectorSystem->setPosition(1.0);
    return true;
}
