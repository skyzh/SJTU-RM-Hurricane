//
// Created by Alex Chi on 2018/11/12.
//

#include "OI.h"
#include "hal.h"
#include "HurricaneChassisSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "CAHRR/src/utils.h"

const int M3508_SPEED_ID = 1;
const int M3508_CURRENT_ID = 2;

HurricaneChassisSystem::HurricaneChassisSystem() : bf(0), lr(0), rot(0) {
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
    this->bf = this->lr = this->rot = 0;
    OK(oi->CANSystem->set(CHASSIS_FL_ID, 0));
    OK(oi->CANSystem->set(CHASSIS_FR_ID, 0));
    OK(oi->CANSystem->set(CHASSIS_BL_ID, 0));
    OK(oi->CANSystem->set(CHASSIS_BR_ID, 0));
    OK(oi->debugSystem->info("CHA", "  ... complete"));
    return true;
}

bool HurricaneChassisSystem::update() {
    double bf = this->bf, lr = this->lr;
    double rotate_speed = (this->rot + bf * 0.01 + lr * 0.01) * 1.0;
    double target[4] = {
            clamp(bf + lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm,
            clamp(-bf + lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm,
            clamp(bf - lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm,
            clamp(-bf - lr + rotate_speed, -2.0, 2.0) / 2.0 * max_rpm
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
    if (!disabled) {
        for (int i = 0; i < 4; i++) {
            double err = target[i] - spd[i];
            int16_t pid = clamp(this->pid_acc[i]->calc(err), -output_limit, output_limit);
            this->ramp_acc[i]->data(pid);
            output[i] = this->ramp_acc[i]->calc(pid);
        }
    }

#ifdef HURRICANE_CHASSIS_DEBUG
    if ((___cnt = ___cnt + 1) % 20 == 0) {
        sprintf(_buf, "out %d %d %d %d", (int16_t)output[0], (int16_t)output[1], (int16_t)output[2], (int16_t)output[3]);
        oi->debugSystem->info("CHA", _buf);
        sprintf(_buf, "tar %f %f %f %f", target[0], target[1], target[2], target[3]);
        // oi->debugSystem->info("CHA", _buf);
        sprintf(_buf, "spd %d %d %d %d", spd[0], spd[1], spd[2], spd[3]);
        oi->debugSystem->info("CHA", _buf);
    }
#endif

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
    this->bf = clamp(bf, -1.0, 1.0);
    this->lr = clamp(lr, -1.0, 1.0);
    this->rot = clamp(rot, -1.0, 1.0);
    return true;
}
