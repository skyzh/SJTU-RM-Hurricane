//
// Created by Alex Chi on 2018/11/12.
//

#include "hal.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "CAHRR/src/utils.h"
#include "Models/Arm.h"
#include "Models/ArmTop.h"
#include "Models/ArmBottom.h"

#include <cmath>


// #define HURRICANE_ARM_DISABLE

const int M2006_ANGLE_ID = 0;
const int M2006_SPEED_ID = 1;

const double PID_SPEED_LIMIT = 1500;
const double PID_CURRENT_LIMIT = 2000;

HurricaneArmSystem::HurricaneArmSystem() : position(0), data_available_bottom(false) {
    // Bottom Arm
    this->arm_bottom = new ArmBottom(
            // Speed P I D and Limit
            0.5, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            30.0, 0.0, 0.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
    this->arm_top = new ArmTop(
            // Speed P I D and Limit
            0.0, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            0.0, 0.0, 0.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
    this->arm_bottom->top = this->arm_top;
    this->arm_top->bottom = this->arm_bottom;
}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("ARM", "arm system initialize"));
    OK(this->arm_bottom->initialize());
    OK(this->arm_top->initialize());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    OK(oi->debugSystem->info("ARM", "  ... complete"));
    return true;
}

bool HurricaneArmSystem::update() {
    OK(this->arm_bottom->update());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, this->arm_bottom->output()));
    return true;
}

bool HurricaneArmSystem::destroy() {
    OK(this->arm_bottom->destroy());
    OK(this->arm_top->destroy());
    oi->CANSystem->set(ARM_BOTTOM_ID, 0);
    return true;
}

bool HurricaneArmSystem::setPosition(double position) {
    this->position = position;
    return true;
}

void HurricaneArmSystem::data() {
    if (this->data_available_bottom ||
        (!this->data_available_bottom && oi->CANSystem->available(ARM_BOTTOM_ID, M2006_ANGLE_ID))) {
        this->data_available_bottom = true;
        this->arm_bottom->feed((int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, M2006_ANGLE_ID),
                               (int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, M2006_SPEED_ID));
    }
}

void HURRICANE_CAN_0_5_6_DATA() {
    HSYS(armSystem) oi->armSystem->data();
}
