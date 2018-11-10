//
// Created by Alex Chi on 2018/11/02.
//

#include <cstdio>
#include "hal.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "HurricaneIMUSystem.h"
#include "CAHRR/src/RotateAccumulator.h"
#include "CAHRR/src/PIDAccumulator.h"

char buf[1000] = "Boot sequence ...";

OI *oi;

extern "C" void charr_bootstrap() {
    oi = new OI;
    oi->boostrap();
}

extern "C" void charr_loop() {
    oi->loop();
}

RotateAccumulator *accumulator = new RotateAccumulator(8192);
PIDDisplacementAccumulator *pid = new PIDDisplacementAccumulator;

const int PID_BOUND = 1000;
int target = -500;

void OI::boostrap() {
    // initialize sequence
    this->debugSystem = new HurricaneDebugSystem;
    this->CANSystem = new HurricaneCANSystem(0);
    this->IMUSystem = new HurricaneIMUSystem;

    OK(this->debugSystem->initialize());

    OK(this->debugSystem->info("OI", "---- booting sequence ----"));

    OK(this->CANSystem->initialize());

    OK(this->IMUSystem->initialize());

    OK(this->debugSystem->info("OI", "    remote control initialize"));
    dbus_uart_init();
    OK(this->debugSystem->info("OI", "      ... complete"));

    OK(this->debugSystem->info("OI", "--- system initialized ---"));

    pid->set_pid(3.0, 0.001, 0.0);
    pid->set_output(-PID_BOUND, PID_BOUND);
    pid->reset();
}

extern rc_info_t rc;
bool d_av = false;

inline double PID_clamp(double result, double min_result, double max_result) {
    return result < min_result ? min_result : (result > max_result ? max_result : result);
}

int cnt = 0;
double Kf = 200.0;

int sgn(int dat) {
    return dat > 0 ? 1 : dat < 0 ? -1 : 0;
}

void OI::loop() {
    HAL_IWDG_Refresh(&hiwdg);
    double error = target - accumulator->get_round();
    double data = pid->calc(error);

    int16_t output = (PID_clamp(data + sgn(error) * Kf, -PID_BOUND, PID_BOUND));
    if (cnt % 5 == 0) {
        sprintf(buf, "cur%3d err%3d out%3d", (int) accumulator->get_round(), (int)error, output);
        this->debugSystem->info("OI", buf);
    }

    if ((cnt = (cnt + 1) % 1000) == 0) {
        target = -target;
    }

    this->CANSystem->set(2, output);
    this->CANSystem->update();

    HAL_Delay(10);
}


void HURRICANE_CAN0_2_DATA() {
    if (!d_av && oi->CANSystem->available(2, 0) || d_av) {
        d_av = true;
        accumulator->data(oi->CANSystem->get(2, 0));
    }
}

extern "C" void hurricane_error_handler(char *file, int line) {
    oi->debugSystem->error("HER", std::string(file) + "@" + std::to_string(line));
}