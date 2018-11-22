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

#include <cmath>


// #define HURRICANE_ARM_DISABLE

const int M2006_ANGLE_ID = 0;
const int M2006_SPEED_ID = 1;

const double PID_SPEED_LIMIT = 1500;
const double PID_CURRENT_LIMIT = 2000;

class ArmBottom : public Arm {
public:
    const double arm_length = 15;

    ArmBottom(double spd_p, double spd_i, double spd_d, double cur_limit, double pos_p, double pos_i, double pos_d,
              double spd_limit, double ramp_limit, double feed_forward) : Arm(spd_p, spd_i, spd_d, cur_limit, pos_p,
                                                                              pos_i, pos_d, spd_limit, ramp_limit,
                                                                              feed_forward) {

    }

    double feed_forward(double current_pos) {
        return 0;
    }
};

HurricaneArmSystem::HurricaneArmSystem() : position(0), data_available_bottom(false) {
    // Bottom Arm
    this->bottom_arm = new ArmBottom(
            // Speed P I D and Limit
            0.5, 0.0, 1.0, PID_CURRENT_LIMIT,
            // Position P I D and Limit
            30.0, 0.0, 0.0, PID_SPEED_LIMIT,
            // Ramp, Feed Forward
            800, 0);
}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("ARM", "arm system initialize"));
    OK(this->bottom_arm->initialize());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    OK(oi->debugSystem->info("ARM", "  ... complete"));
    return true;
}

bool HurricaneArmSystem::update() {
    OK(this->bottom_arm->update());
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, this->bottom_arm->output()));
    return true;
}

bool HurricaneArmSystem::destroy() {
    OK(this->bottom_arm->destroy());
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
        this->bottom_arm->feed((int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, M2006_ANGLE_ID),
                               (int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, M2006_SPEED_ID));
    }
}

void HURRICANE_CAN_0_5_6_DATA() {
    HSYS(armSystem) oi->armSystem->data();
}
