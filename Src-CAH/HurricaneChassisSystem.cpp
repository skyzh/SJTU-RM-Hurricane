//
// Created by Alex Chi on 2018/11/12.
//

#include "HurricaneChassisSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "hal.h"

const int M3508_SPEED_ID = 2;
const double Kp = 1.0, Ki = 0.0, Kd = 0.0;
const int output_limit = 10000;
const int max_rpm = 1000;

inline double chassis_clamp(double in, double min, double max) {
    return (in < min ? min : (in > max ? max : in));
}

HurricaneChassisSystem::HurricaneChassisSystem() {
    this->fl = new PIDRateAccumulator();
    this->fr = new PIDRateAccumulator();
    this->bl = new PIDRateAccumulator();
    this->br = new PIDRateAccumulator();
    this->fl->set_pid(Kp, Ki, Kd);
    this->fr->set_pid(Kp, Ki, Kd);
    this->bl->set_pid(Kp, Ki, Kd);
    this->br->set_pid(Kp, Ki, Kd);
    this->fl->set_output(-output_limit, output_limit);
    this->fr->set_output(-output_limit, output_limit);
    this->bl->set_output(-output_limit, output_limit);
    this->br->set_output(-output_limit, output_limit);
}

bool HurricaneChassisSystem::initialize() {
    OK(oi->debugSystem->info("CHA", "chassis system initialize"));
    this->fl->reset();
    this->fr->reset();
    this->bl->reset();
    this->br->reset();
    OK(oi->debugSystem->info("CHA", "  ... complete"));
    return true;
}

bool HurricaneChassisSystem::update() {
    double rotate_speed = this->rot + this->bf * 0.01 + this->lr * 0.01;
    double bf = this->bf, lr = this->lr;
    double target[4] = {
            (bf + lr + rotate_speed) / 2.0 * max_rpm,
            (-bf + lr + rotate_speed) / 2.0 * max_rpm,
            (bf - lr + rotate_speed) / 2.0 * max_rpm,
            (-bf - lr + rotate_speed) / 2.0 * max_rpm
    };
    double err[4] = {
            target[0] - oi->CANSystem->get(CHASSIS_FL_ID, M3508_SPEED_ID),
            target[1] - oi->CANSystem->get(CHASSIS_FR_ID, M3508_SPEED_ID),
            target[2] - oi->CANSystem->get(CHASSIS_BL_ID, M3508_SPEED_ID),
            target[3] - oi->CANSystem->get(CHASSIS_BR_ID, M3508_SPEED_ID)
    };
    OK(oi->CANSystem->set(CHASSIS_FL_ID, uint16_t((int16_t) chassis_clamp(this->fl->calc(err[0]), -output_limit, output_limit))));
    OK(oi->CANSystem->set(CHASSIS_FR_ID, uint16_t((int16_t) chassis_clamp(this->fl->calc(err[1]), -output_limit, output_limit))));
    OK(oi->CANSystem->set(CHASSIS_BL_ID, uint16_t((int16_t) chassis_clamp(this->fl->calc(err[2]), -output_limit, output_limit))));
    OK(oi->CANSystem->set(CHASSIS_BR_ID, uint16_t((int16_t) chassis_clamp(this->fl->calc(err[3]), -output_limit, output_limit))));
    return true;
}

bool HurricaneChassisSystem::destroy() {
    OK(oi->CANSystem->set(CHASSIS_FL_ID, 0));
    OK(oi->CANSystem->set(CHASSIS_FR_ID, 0));
    OK(oi->CANSystem->set(CHASSIS_BL_ID, 0));
    OK(oi->CANSystem->set(CHASSIS_BR_ID, 0));
    return true;
}

bool HurricaneChassisSystem::setSpeed(double bf, double lr, double rot) {
    this->bf = chassis_clamp(bf, -1.0, 1.0);
    this->lr = chassis_clamp(lr, -1.0, 1.0);
    this->rot = chassis_clamp(rot, -1.0, 1.0);
    return true;
}
