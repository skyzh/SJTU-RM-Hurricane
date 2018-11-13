//
// Created by Alex Chi on 2018/11/12.
//

#include "HurricaneChassisSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "hal.h"

HurricaneChassisSystem::HurricaneChassisSystem() : ChassisSystem() {

}

bool HurricaneChassisSystem::initialize() {
    OK(oi->debugSystem->info("CHA", "chassis system initialize"));
    OK(oi->debugSystem->info("CHA", "  ... complete"));
    return true;
}

bool HurricaneChassisSystem::update() {
    return true;
}

bool HurricaneChassisSystem::destroy() {
    return true;
}

bool HurricaneChassisSystem::setSpeed(double bf, double lr) {
    return false;
}
