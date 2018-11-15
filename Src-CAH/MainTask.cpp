//
// Created by Alex Chi on 2018/11/12.
//

#include "CAHRR/src/RotateAccumulator.h"
#include "CAHRR/src/PIDAccumulator.h"
#include "MainTask.h"
#include "OI.h"
#include "HurricaneIMUSystem.h"
#include "HurricaneCANSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneUltrasonicSystem.h"

RotateAccumulator *accumulator = new RotateAccumulator(8192);
PIDDisplacementAccumulator *pid = new PIDDisplacementAccumulator;

const int PID_BOUND = 500;
int target = -300;
int cnt = 0;

double Kf = 200.0;

char buf[1000];
bool d_av;

int sgn(int dat) {
    return dat > 0 ? 1 : dat < 0 ? -1 : 0;
}
inline double PID_clamp(double result, double min_result, double max_result) {
    return result < min_result ? min_result : (result > max_result ? max_result : result);
}

MainTask::MainTask() : Task() {

}

bool MainTask::initialize() {
    pid->set_pid(3.0, 0.0005, 0.1);
    pid->set_output(-PID_BOUND, PID_BOUND);
    pid->reset();
    return true;
}

bool MainTask::destroy() {
    return false;
}

bool MainTask::isEnd() {
    return Task::isEnd();
}

int __cnt = 0, __res = 0;
bool MainTask::update() {
    double error = target - accumulator->get_round();
    double data = pid->calc(error);

    int16_t output = (PID_clamp(data + sgn(error) * Kf, -PID_BOUND, PID_BOUND));
    if (cnt % 20 == 0) {
        // sprintf(buf, "cur%3d err%3d out%3d %5d", (int) accumulator->get_round(), (int)error, output, (int) oi->usSystemChassis->distance() * 100);
        sprintf(buf, "%d cnt%d res%d", oi->usSystemChassis->time(), __cnt, __res);
        // oi->debugSystem->info("OI", buf);
        oi->debugSystem->info("MTK", buf);
    }

    if ((cnt = (cnt + 1) % 1000) == 0) {
        target = -target;

    }
    if (HAL_GPIO_ReadPin(US_ECHO_GPIO_Port, US_ECHO_Pin) == GPIO_PIN_SET) {
        __cnt++;
    } else if (HAL_GPIO_ReadPin(US_ECHO_GPIO_Port, US_ECHO_Pin) == GPIO_PIN_RESET) {
        __res++;
    }

    oi->CANSystem->set(2, 0);
    return true;
}

MainTask::~MainTask() {

}


void HURRICANE_CAN0_2_DATA() {
    if (!d_av && oi->CANSystem->available(2, 0) || d_av) {
        d_av = true;
        if (accumulator) accumulator->data(oi->CANSystem->get(2, 0));
    }
}

Task* mainTask() {
    return new MainTask;
}
