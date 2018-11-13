//
// Created by Alex Chi on 2018/11/12.
//

#include "HurricaneArmSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "hal.h"

HurricaneArmSystem::HurricaneArmSystem() {}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("CHA", "arm system initialize"));
    OK(oi->debugSystem->info("CHA", "  ... complete"));
    return true;
}

bool HurricaneArmSystem::update() {
    return true;
}

bool HurricaneArmSystem::destroy() {
    return true;
}

bool HurricaneArmSystem::setPosition(double location) {
    return false;
}
