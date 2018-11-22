//
// Created by Alex Chi on 2018/11/22.
//

#ifndef HURRICANE_ARMTOP_H
#define HURRICANE_ARMTOP_H

#include "Arm.h"
#include <cmath>

class ArmBottom;

class ArmTop : public Arm {
private:
    const double PI = acos(-1);
public:
    ArmBottom *bottom;
    const double arm_length = 18;
    const double offset_angle = PI / 180.0 * 90.0;
    const double mass = (PI * (1.2 * 1.2 - 1.15 * 1.15)) * arm_length * 7.7; // gram
    const double g = 9.8;
    const double clockwise = 1;

    ArmTop(double spd_p, double spd_i, double spd_d, double cur_limit, double pos_p, double pos_i, double pos_d,
           double spd_limit, double ramp_limit, double feed_forward) :
            Arm(spd_p, spd_i, spd_d, cur_limit, pos_p, pos_i, pos_d, spd_limit, ramp_limit, feed_forward) {
    }

    double real_angle();

    double expected_pos(double angle);

    double delta_x();

    double delta_y();

    double gravity();

    double feed_forward();
};

#endif //HURRICANE_ARMTOP_H
