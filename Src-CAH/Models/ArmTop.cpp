//
// Created by Alex Chi on 2018/11/22.
//

#include "ArmTop.h"
#include "ArmBottom.h"

double ArmTop::real_angle() {
    return (this->current_pos / 18.0 * PI - offset_angle) * clockwise + bottom->real_angle();
}

double ArmTop::expected_pos(double angle) {
    // todo: check clockwise
    return (((angle - bottom->real_angle()) * clockwise) + offset_angle) / PI * 18.0;
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
