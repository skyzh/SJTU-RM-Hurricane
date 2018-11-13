//
// Created by Alex Chi on 2018/11/12.
//

#include "TestTask.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "CAHRR/src/PIDAccumulator.h"
#include "CAHRR/src/RotateAccumulator.h"

PIDRateAccumulator *pid;
RotateAccumulator *accumulator;

int lst_time;
double lst_round;
bool d_av = false;
const double MAX_SPEED = 1000;

bool TestTask::initialize() {
    pid = new PIDRateAccumulator();
    pid->set_pid(0.07, 0, 0.1);
    pid->set_output(-MAX_SPEED, MAX_SPEED);
    pid->reset();
    accumulator = new RotateAccumulator(CAN_MAX_ANGLE);
    lst_time = HAL_GetTick() - 1;
    lst_round = 0;
    return true;
}

bool TestTask::destroy() {
    oi->CANSystem->set(2, 0);
    return true;
}

double target_speed = 100;

inline double BOUND(double d, double mi, double ma) {
    return (d < mi ? mi : (d > ma ? ma : d));
}

char buf[1000] = "";

int cnt = 0;
bool TestTask::update() {
    double n_round = accumulator->get_round() + (double)accumulator->get_overflow() / CAN_MAX_ANGLE;
    double _d_round = n_round - lst_round;
    lst_round = n_round;

    int _d_time = HAL_GetTick() - lst_time;
    lst_time = HAL_GetTick();

    double _d_speed = _d_round / _d_time * 1000.0;
    double _err = target_speed - _d_speed;

    int16_t output = (int16_t) BOUND(pid->calc(_err), -MAX_SPEED, MAX_SPEED);

    cnt = (cnt + 1) % 700;
    if (cnt % 20 == 0) {
        sprintf(buf, "%.2f %.2f %d", n_round, _d_speed, output);
        oi->debugSystem->info("TAK", buf);
    }
    if (cnt == 0) {
        target_speed = -target_speed;
    }


    oi->CANSystem->set(2, output);
    return true;
}
