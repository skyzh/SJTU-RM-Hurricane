//
// Created by Alex Chi on 2018/11/22.
//

#include "Arm.h"
#include "CAHRR/src/utils.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"

const int M2006_MAX_ANGLE = 8192;
#define HURRICANE_ARM_DEBUG

bool Arm::initialize() {
    this->pid_rate.reset();
    this->pid_position.reset();
    this->ramp.reset();
    this->speed.reset();
    this->target_pos = 0;
    this->target_current = 0;
    // DO NOT RESET ACCUMULATOR because data may come before initialization
    return true;
}

bool Arm::destroy() {
    this->target_current = 0;
    return true;
}

bool Arm::update() {
    double current_pos = this->accumulator.get_round() + this->accumulator.get_overflow() / (double) M2006_MAX_ANGLE;
    double target_spd = clamp(this->pid_position.calc(target_pos - current_pos), -spd_limit, spd_limit);
    double current_spd = this->speed.sum();
    double target_current = clamp(this->pid_rate.calc(target_spd - current_spd), -cur_limit, cur_limit);
    this->ramp.data(target_current);
    double output = clamp(this->ramp.calc(target_current) + this->feed_forward(current_pos) * this->Kf, -cur_limit, cur_limit);
    this->target_current = (int16_t) output;
#ifdef HURRICANE_ARM_DEBUG
    HDEBUG_BEGIN(100)
            static char _buf[1000];
            sprintf(_buf, "tpos %f, pos %f, spd %f, out %f", this->target_pos, current_pos, target_spd, output);
            oi->debugSystem->info("ARM", _buf);
    HDEBUG_END()
#endif
    return true;
}

int16_t Arm::output() {
    return target_current;
}

Arm::Arm(double spd_p, double spd_i, double spd_d, double cur_limit,
         double pos_p, double pos_i, double pos_d, double spd_limit,
         double ramp_limit, double feed_forward) :
        ramp(ramp_limit), accumulator(M2006_MAX_ANGLE), cur_limit(cur_limit), spd_limit(spd_limit),
        Kf(feed_forward), target_pos(0), target_current(0) {
    // Position PID
    this->pid_position.set_pid(pos_p, pos_i, pos_d);
    this->pid_position.set_output(-spd_limit, spd_limit);
    this->pid_position.reset();
    // Rate PID
    this->pid_rate.set_pid(spd_p, spd_i, spd_d);
    this->pid_rate.set_output(-cur_limit, cur_limit);
    this->pid_rate.reset();
    // Ramp: do nothing
    // Accumulator
    this->accumulator.reset();
    // Speed
    this->speed.reset();
}

bool Arm::feed(int16_t ang, int16_t spd) {
    this->accumulator.data(ang);
    this->speed.data(spd);
    return true;
}

bool Arm::set_pos(double pos) {
    this->target_pos = pos;
    return true;
}
