//
// Created by Alex Chi on 2018/11/12.
//

#include "HurricaneChassisSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "hal.h"

const int M3508_SPEED_ID = 1;
const int M3508_CURRENT_ID = 2;
const double Kp = 3.0, Ki = 0, Kd = 1.5;
const int output_limit = 15000;
const int max_rpm = 5000;
const double ramp_limit = 80;

template <typename T>
inline T chassis_clamp(T in, T min, T max) {
    return (in < min ? min : (in > max ? max : in));
}

HurricaneChassisSystem::HurricaneChassisSystem() {
    for (int i = 0; i < 4; i++) {
        this->pid_acc[i] = new PIDRateAccumulator();
        this->pid_acc[i]->set_pid(Kp, Ki, Kd);
        this->pid_acc[i]->set_output(-output_limit, output_limit);
        this->ramp_acc[i] = new RampAccumulator<double>(ramp_limit);
    }
}

bool HurricaneChassisSystem::initialize() {
    OK(oi->debugSystem->info("CHA", "chassis system initialize"));
    for (int i = 0; i < 4; i++) {
        this->pid_acc[i]->reset();
        this->ramp_acc[i]->reset();
    }
    OK(oi->debugSystem->info("CHA", "  ... complete"));
    return true;
}

char _buf[1000];
int ___cnt = 0;
bool HurricaneChassisSystem::update() {
    double bf = this->bf, lr = this->lr;
    double rotate_speed = (this->rot + bf * 0.01 + lr * 0.01) * 1.0;
    double target[4] = {
            chassis_clamp(bf + lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm,
            chassis_clamp(-bf + lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm,
            chassis_clamp(bf - lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm,
            chassis_clamp(-bf - lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm
    };
    int16_t spd[4] = {
        (int16_t) oi->CANSystem->get(CHASSIS_FL_ID, M3508_SPEED_ID),
        (int16_t) oi->CANSystem->get(CHASSIS_FR_ID, M3508_SPEED_ID),
        (int16_t) oi->CANSystem->get(CHASSIS_BL_ID, M3508_SPEED_ID),
        (int16_t) oi->CANSystem->get(CHASSIS_BR_ID, M3508_SPEED_ID)
    };
    int16_t current[4] = {
        (int16_t) oi->CANSystem->get(CHASSIS_FL_ID, M3508_CURRENT_ID),
        (int16_t) oi->CANSystem->get(CHASSIS_FR_ID, M3508_CURRENT_ID),
        (int16_t) oi->CANSystem->get(CHASSIS_BL_ID, M3508_CURRENT_ID),
        (int16_t) oi->CANSystem->get(CHASSIS_BR_ID, M3508_CURRENT_ID)
    };
    int16_t output[4] = { 0 };

    for (int i = 0; i < 4; i++) {
        double err = target[i] - spd[i];
        int16_t pid = chassis_clamp<double>(this->pid_acc[i]->calc(err), -output_limit, output_limit);
        this->ramp_acc[i]->data(pid);
        output[i] = this->ramp_acc[i]->calc(pid);
    }

    if ((___cnt = ___cnt + 1) % 20 == 0) {
        sprintf(_buf, "out %d %d %d %d", (int16_t)output[0], (int16_t)output[1], (int16_t)output[2], (int16_t)output[3]);
        oi->debugSystem->info("CHA", _buf);
        sprintf(_buf, "tar %f %f %f %f", target[0], target[1], target[2], target[3]);
        // oi->debugSystem->info("CHA", _buf);
        sprintf(_buf, "spd %d %d %d %d", spd[0], spd[1], spd[2], spd[3]);
        oi->debugSystem->info("CHA", _buf);
    }

    OK(oi->CANSystem->set(CHASSIS_FL_ID, output[0]));
    OK(oi->CANSystem->set(CHASSIS_FR_ID, output[1]));
    OK(oi->CANSystem->set(CHASSIS_BL_ID, output[2]));
    OK(oi->CANSystem->set(CHASSIS_BR_ID, output[3]));
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
