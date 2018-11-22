//
// Created by Alex Chi on 2018/11/22.
//

#include "ArmTop.h"
#include "ArmBottom.h"
#include "HurricaneDebugSystem.h"
#include "OI.h"
#include "CAHRR/src/utils.h"

#define HURRICANE_ARM_DEBUG
const int M2006_MAX_ANGLE = 8192;

double ArmTop::real_angle() {
    return (this->current_pos / 18.0 * M_PI + offset_angle) * clockwise + bottom->real_angle();
}

double ArmTop::expected_pos(double angle) {
    // todo: check clockwise
    return (((angle - bottom->real_angle()) * clockwise) - offset_angle) / M_PI * 18.0;
}

double ArmTop::delta_x() {
    return cos(this->real_angle()) * this->arm_length;
}

double ArmTop::delta_y() {
    return sin(this->real_angle()) * this->arm_length;
}

double ArmTop::feed_forward() {
    // todo: check clockwise
    return this->delta_x() / 2.0 * gravity();
}

double ArmTop::gravity() {
     // TODO: calculate claw weight
    // return mass * g / 1000.0;
    return 0;
}

bool ArmTop::update() {
    current_pos = this->accumulator.get_round() + this->accumulator.get_overflow() / (double) M2006_MAX_ANGLE;
    target_spd = 20; /*clamp(this->pid_position.calc(target_pos - current_pos), -spd_limit, spd_limit)*/
    current_spd = this->speed.sum();
    target_current = clamp(this->pid_rate.calc(target_spd - current_spd), -cur_limit, cur_limit);
    this->ramp.data(target_current);
    double output = clamp(this->ramp.calc(target_current) + this->feed_forward() * this->Kf, -cur_limit, cur_limit);
    this->target_output = (int16_t) output;
#ifdef HURRICANE_ARM_DEBUG
    HDEBUG_BEGIN(100)
            static char _buf[1000];
            sprintf(_buf, "tpos %.2f pos %.2f, tspd %.2f spd%.2f, ang %.2f %.2f dx %.2f, out %d",
                    target_pos, current_pos, target_spd,
                    current_spd, bottom->real_angle() / M_PI * 180.0, real_angle() / PI * 180, delta_x(), target_output);
            oi->debugSystem->info("ARM_T", _buf);
    HDEBUG_END()
#endif
    return true;
}
