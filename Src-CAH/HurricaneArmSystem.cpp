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
#include "Models/ArmBase.h"

#include <cmath>

// #define HURRICANE_ARM_DISABLE

const int M2006_ANGLE_ID = 0;
const int M2006_SPEED_ID = 1;
const int M3508_ANGLE_ID = 0;
const int M3508_SPEED_ID = 1;

const double PID_SPEED_LIMIT = 800;
const double PID_CURRENT_LIMIT = 800;

HurricaneArmSystem::HurricaneArmSystem() : position(0),
                                           data_available_bottom(false),
                                           data_available_base(false),
                                           data_available_top(false) {
    // Bottom Arm
    this->arm_bottom = new ArmBottom(
            // Speed P I D and Limit
            1.0, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            30.0, 0.0, 10.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("ARM", "arm system initialize"));
    OK(this->arm_bottom->initialize());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    OK(oi->debugSystem->info("ARM", "  ... complete"));
    return true;
}

bool HurricaneArmSystem::update() {
    OK(this->arm_bottom->update());
    if (!disabled) {
        OK(oi->CANSystem->set(ARM_BOTTOM_ID, this->arm_bottom->output()));
    } else {
        OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    }
    return true;
}

bool HurricaneArmSystem::destroy() {
    OK(this->arm_bottom->destroy());
    return true;
}

bool HurricaneArmSystem::setPosition(double position) {
    this->position = position;
    this->arm_bottom->set_pos(this->arm_bottom->expected_pos((this->position - 0.5) * M_PI));
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

void HURRICANE_CAN_0_5_6_7_DATA() {
    HSYS(armSystem) oi->armSystem->data();
}

