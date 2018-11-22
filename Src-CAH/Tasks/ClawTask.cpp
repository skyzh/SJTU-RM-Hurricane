//
// Created by Alex Chi on 2018/11/22.
//

#include "ClawTask.h"
#include "hal.h"
#include "CAHRR/src/utils.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneClawSystem.h"
#include "HurricaneDebugSystem.h"

ClawTask::ClawTask() : Task() {

}

bool ClawTask::initialize() {
    oi->debugSystem->info("CLA", "init");

    return true;
}

bool ClawTask::destroy() {
    oi->debugSystem->info("CLA", "deinit");
    return true;
}

bool ClawTask::isEnd() {
    return false;
}

bool ClawTask::update() {
    // oi->clawSystem->setPosition(map_range<double>(oi->remoteSystem->getAxis(0), -660, 660, 0.0, 1.0));
    return true;
}
