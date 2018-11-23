//
// Created by Alex Chi on 2018/11/22.
//

#include "ArmBottom.h"
#include "ArmTop.h"
#include "HurricaneDebugSystem.h"
#include "OI.h"
#include <cmath>

const double ARM_BASE_GEAR_RATIO = 27.0 / 8.0;
#define HURRICANE_ARM_DEBUG

double ArmBottom::real_angle() {
    return (this->current_pos / 18.0 * M_PI + offset_angle) * clockwise;
}

double ArmBottom::expected_pos(double angle) {
    return ((angle * clockwise) - offset_angle) / M_PI * 18.0;
}

double ArmBottom::delta_x() {
    return cos(this->real_angle()) * this->arm_length;
}

double ArmBottom::delta_y() {
    return sin(this->real_angle()) * this->arm_length;
}

double ArmBottom::gravity() {
    return mass * g / 1000.0;
}

double ArmBottom::torque() {
    return this->delta_x() / 2.0 * this->gravity();
}

double ArmBottom::feed_forward() {
    // todo: check clockwise
    return 0; // this->torque() + (this->delta_x() + top->delta_x() / 2) * top->gravity();
}

bool ArmBottom::update() {
    bool result = Arm::update();
#ifdef HURRICANE_ARM_DEBUG
    HDEBUG_BEGIN(100)
            static char _buf[1000];
            sprintf(_buf, "tpos %.2f pos %.2f, tspd %.2f spd%.2f, ang %.2f dx %.2f, out %d",
                    target_pos, current_pos, target_spd,
                    current_spd, real_angle() / PI * 180, delta_x(), target_output);
            oi->debugSystem->info("ARM_B", _buf);
    HDEBUG_END()
#endif
    return result;
}
