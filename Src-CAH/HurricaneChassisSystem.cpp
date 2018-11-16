//
// Created by Alex Chi on 2018/11/12.
//

#include "HurricaneChassisSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "hal.h"

const int M3508_SPEED_ID = 1;
const double Kp = 0.2, Ki = 0.0, Kd = 0.0;
const int output_limit = 500;
const int max_rpm = 500;
const int ramp_limit = 10;

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
    this->rfl = new RampAccumulator(ramp_limit);
    this->rfr = new RampAccumulator(ramp_limit);
    this->rbl = new RampAccumulator(ramp_limit);
    this->rbr = new RampAccumulator(ramp_limit);
    this->rfl->reset();
    this->rfr->reset();
    this->rbl->reset();
    this->rbr->reset();
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

char _buf[1000];
int ___cnt = 0;
bool HurricaneChassisSystem::update() {
    double bf = this->bf, lr = this->lr;
    double rotate_speed = this->rot + bf * 0.01 + lr * 0.01;
    double target[4] = {
            (bf + lr + rotate_speed) / 2.0 * max_rpm,
            (-bf + lr + rotate_speed) / 2.0 * max_rpm,
            (bf - lr + rotate_speed) / 2.0 * max_rpm,
            (-bf - lr + rotate_speed) / 2.0 * max_rpm
    };
    int16_t data[4] = {
            (int16_t) oi->CANSystem->get(CHASSIS_FL_ID, M3508_SPEED_ID),
            (int16_t) oi->CANSystem->get(CHASSIS_FR_ID, M3508_SPEED_ID),
            (int16_t) oi->CANSystem->get(CHASSIS_BL_ID, M3508_SPEED_ID),
            (int16_t) oi->CANSystem->get(CHASSIS_BR_ID, M3508_SPEED_ID)
    };
    double err[4] = {
            target[0] - data[0],
            target[1] - data[1],
            target[2] - data[2],
            target[3] - data[3]
    };
    int16_t pid[4] = {
            (int16_t) chassis_clamp(this->fl->calc(err[0]), -output_limit, output_limit),
            (int16_t) chassis_clamp(this->fr->calc(err[1]), -output_limit, output_limit),
            (int16_t) chassis_clamp(this->bl->calc(err[2]), -output_limit, output_limit),
            (int16_t) chassis_clamp(this->br->calc(err[3]), -output_limit, output_limit)
    };
    int16_t output[4] = {
            (int16_t) this->rfl->calc(pid[0]),
            (int16_t) this->rfl->calc(pid[1]),
            (int16_t) this->rfl->calc(pid[2]),
            (int16_t) this->rfl->calc(pid[3])
    };
    if ((___cnt = ___cnt + 1) % 10 == 0) {
        sprintf(_buf, "out %d %d %d %d", (int16_t)output[0], (int16_t)output[1], (int16_t)output[2], (int16_t)output[3]);
        oi->debugSystem->info("CHA", _buf);
        sprintf(_buf, "tar %f %f %f %f", target[0], target[1], target[2], target[3]);
        oi->debugSystem->info("CHA", _buf);
        sprintf(_buf, "err %f %f %f %f", err[0], err[1], err[2], err[3]);
        oi->debugSystem->info("CHA", _buf);
    }

    OK(oi->CANSystem->set(CHASSIS_FL_ID, 200));
    OK(oi->CANSystem->set(CHASSIS_FR_ID, 200));
    OK(oi->CANSystem->set(CHASSIS_BL_ID, 200));
    OK(oi->CANSystem->set(CHASSIS_BR_ID, 200));
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
