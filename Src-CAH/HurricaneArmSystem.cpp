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
#define HURRICANE_ARM_DISABLE

const int ramp_limit = 2000;
const double Kp = 20.0, Ki = 0.01, Kd = 1.0;
const double output_limit = 2000;
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

HurricaneArmSystem::HurricaneArmSystem() : position(0), data_available_bottom(false) {
    this->pid_bottom = new PIDDisplacementAccumulator();
    this->ramp_bottom = new RampAccumulator<double>(ramp_limit);
    this->pid_bottom->set_pid(Kp, Ki, Kd);
    this->pid_bottom->set_output(-output_limit, output_limit);
    this->accumulator_bottom = new RotateAccumulator(M2006_MAX_ANGLE);
    this->accumulator_bottom->reset();
}

bool HurricaneArmSystem::initialize() {
    OK(oi->debugSystem->info("CHA", "arm system initialize"));
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, 0));
    // reset pid and ramp
    this->pid_bottom->reset();
    this->ramp_bottom->reset();
    this->accumulator_bottom->reset();
    this->position = 0;
    OK(oi->debugSystem->info("CHA", "  ... complete"));
    this->position = 0;
    return true;
}

DeltaAccumulator <int> *timeDelta = new DeltaAccumulator<int>();
DeltaAccumulator <double> *posDelta = new DeltaAccumulator<double>();

bool HurricaneArmSystem::update() {

    double arm_bottom_pos = this->accumulator_bottom->get_round() +
                            this->accumulator_bottom->get_overflow() / (double) M2006_MAX_ANGLE;
    double arm_bottom_err = this->position - arm_bottom_pos;

    int16_t arm_bottom_pid = clamp(this->pid_bottom->calc(arm_bottom_err), -output_limit, output_limit);
    this->ramp_bottom->data(arm_bottom_pid);
    int16_t arm_bottom_output = this->ramp_bottom->calc(arm_bottom_pid);
    int16_t feed_forward = (sin(arm_bottom_pos / 36.0 * 2 * PI)) * Kf;
    arm_bottom_output = clamp<double>(arm_bottom_output + feed_forward, -output_limit, output_limit);
#ifdef HURRICANE_ARM_DEBUG
    HDEBUG_BEGIN(100)
            sprintf(_buf, "ff %d pos%f err%f out%d tar%f out%d", feed_forward,
                    arm_bottom_pos, arm_bottom_err, arm_bottom_output, position, (int16_t)oi->CANSystem->get(ARM_BOTTOM_ID, 2));
            /*sprintf(_buf, "spd %f tq %d", posDelta->delta(arm_bottom_pos) / timeDelta->delta(HAL_GetTick()) * 1000.0, (int16_t) oi->CANSystem->get(ARM_BOTTOM_ID, 2);*/
            oi->debugSystem->info("CHA", _buf);
    HDEBUG_END()
#endif
#ifndef HURRICANE_ARM_DISABLE
    OK(oi->CANSystem->set(ARM_BOTTOM_ID, arm_bottom_output));
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
    if (!this->data_available_bottom && oi->CANSystem->available(ARM_BOTTOM_ID, M2006_ANGLE_ID) ||
        this->data_available_bottom) {
        this->data_available_bottom = true;
        this->accumulator_bottom->data(oi->CANSystem->get(ARM_BOTTOM_ID, M2006_ANGLE_ID));
    }
}

void HURRICANE_CAN_0_5_DATA() {
    if (oi && oi->armSystem) oi->armSystem->data();
}
