//
// Created by Alex Chi on 2018/11/22.
//

#include "ArmDisableTask.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneDebugSystem.h"

ArmDisableTask::ArmDisableTask() : Task() {

}

bool ArmDisableTask::initialize() {
    oi->armSystem->disabled = true;
    oi->debugSystem->info("ARM", "disabled");
    return true;
}

bool ArmDisableTask::destroy() {
    oi->armSystem->disabled = false;
    oi->debugSystem->info("ARM", "enabled");
    return true;
}

bool ArmDisableTask::isEnd() {
    return false;
}

bool ArmDisableTask::update() {
    return true;
}
