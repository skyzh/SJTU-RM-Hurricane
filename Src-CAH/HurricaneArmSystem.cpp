//
// Created by Alex Chi on 2018/11/12.
//

#include "hal.h"
#include "OI.h"
#include "HurricaneArmSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "CAHRR/src/utils.h"
#include "CAHRR/src/DeltaAccumulator.h"

#include <cmath>

#define HURRICANE_ARM_DEBUG
// #define HURRICANE_ARM_DISABLE

const int M2006_ANGLE_ID = 0;
const int M2006_MAX_ANGLE = 8192;
const double Kf = 500.0;
const double PI = acos(-1);
char _buf[1000];
int status = 0;

int sgn(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

const double PID_SPEED_LIMIT = 10;
const double PID_CURRENT_LIMIT = 3000;

HurricaneArmSystem::HurricaneArmSystem() : position(0), data_available_bottom(false) {
    // Bottom Arm
    // -- Position PID
    this->pid_bottom_position = new PIDDisplacementAccumulator();
    this->pid_bottom_position->set_pid(1.0, 0.0, 0.0);
    this->pid_bottom_position->set_output(-PID_SPEED_LIMIT, PID_SPEED_LIMIT);
    // -- Rate PID
    this->pid_bottom_rate = new PIDRateAccumulator();
    this->pid_bottom_rate->set_pid(1.0, 0.0, 0.0);
    this->pid_bottom_rate->set_output(-PID_CURRENT_LIMIT, PID_CURRENT_LIMIT);
    // -- Ramp
    this->ramp_bottom = new RampAccumulator<double>(800);
    this->accumulator_bottom = new RotateAccumulator(M2006_MAX_ANGLE);
    this->accumulator_bottom->reset();
    // -- Delta Speed
    this->delta_bottom = new DeltaAccumulator<double>();
    this->delta_time = new DeltaAccumulator<int>();
}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("ARM", "arm system initialize"));
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    // reset pid and ramp
    // Bottom Arm
    this->pid_bottom_rate->reset();
    this->pid_bottom_position->reset();
    this->ramp_bottom->reset();
    this->accumulator_bottom->reset();
    this->delta_bottom->reset();
    // Time
    this->delta_time->reset();
    this->delta_time->delta(HAL_GetTick());
    OK(oi->debugSystem->info("ARM", "  ... complete"));
    this->position = 0;
    return true;
}

/*
 * TODO: DEBUG Sequence:
 * [ ] New Event-driven System
 * [ ] Ramp (Chassis)
 * [ ] Test Max Spinning Speed
 * [ ] Compare Delta Pos and Speed
 * [ ] Rate PID
 * [ ] Rate Official PID
 * [ ] Rate PID in Data Feedback
 * [ ] Position PID
 */

bool HurricaneArmSystem::update() {

    double arm_bottom_pos = this->accumulator_bottom->get_round() +
                            this->accumulator_bottom->get_overflow() / (double) M2006_MAX_ANGLE;
    double arm_bottom_speed = this->delta_bottom->delta(arm_bottom_pos) * 1000.0 / this->delta_time->delta(HAL_GetTick());

    double arm_bottom_pid_target_speed = clamp(this->pid_bottom_position->calc(this->position - arm_bottom_pos),
                                               -PID_SPEED_LIMIT, PID_SPEED_LIMIT);
    double arm_bottom_pid_target_current = clamp(arm_bottom_pid_target_speed - arm_bottom_speed,
                                                 -PID_CURRENT_LIMIT, PID_CURRENT_LIMIT);
    double arm_bottom_output = this->ramp_bottom->calc(arm_bottom_pid_target_current);

    this->ramp_bottom->data(arm_bottom_pid_target_current);
#ifdef HURRICANE_ARM_DEBUG
    HDEBUG_BEGIN(100)
            sprintf(_buf, "pos %f, spd %f tspd %f, out %f, tq %d", arm_bottom_pos, arm_bottom_speed, arm_bottom_pid_target_speed, arm_bottom_output,
                    (int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, 2));
            oi->debugSystem->info("ARM", _buf);
    HDEBUG_END()
#endif
#ifndef HURRICANE_ARM_DISABLE
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 400));
#endif
    return true;
}

bool HurricaneArmSystem::destroy() {
    oi->CANSystem->set(ARM_BOTTOM_ID, 0);
    return true;
}

bool HurricaneArmSystem::setPosition(double position) {
    this->position = position;
    return true;
}

void HurricaneArmSystem::data() {
    if (this->data_available_bottom ||
        !this->data_available_bottom && oi->CANSystem->available(ARM_BOTTOM_ID, M2006_ANGLE_ID)) {
        this->data_available_bottom = true;
        this->accumulator_bottom->data(oi->CANSystem->get(ARM_BOTTOM_ID, M2006_ANGLE_ID));
    }
}

void HURRICANE_CAN_0_5_DATA() {
    HSYS(armSystem) oi->armSystem->data();
}
