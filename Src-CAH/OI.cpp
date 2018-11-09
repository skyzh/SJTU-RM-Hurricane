//
// Created by Alex Chi on 2018/11/02.
//

#include <cstdio>
#include "hal.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "CAHRR/Src/RotateAccumulator.h"

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

void OI::boostrap() {
    // initialize sequence
    this->debugSystem = new HurricaneDebugSystem;
    this->CANSystem = new HurricaneCANSystem;
    OK(this->debugSystem->initialize());

    OK(this->debugSystem->info("OI", "booting sequence"));

    OK(this->debugSystem->info("OI", "    MPU initialize"));
    mpu_device_init();
    init_quaternion();
    OK(this->debugSystem->info("OI", "      ... complete"));

    OK(this->CANSystem->initialize(0));

    OK(this->debugSystem->info("OI", "    remote control initialize"));
    dbus_uart_init();
    OK(this->debugSystem->info("OI", "      ... complete"));

    OK(this->debugSystem->info("OI", "  ... complete"));
}

extern rc_info_t rc;
bool d_av = false;
void OI::loop() {
    HAL_IWDG_Refresh(&hiwdg);
    mpu_get_data();
    imu_ahrs_update();
    imu_attitude_update();
    // std::sprintf(buf, "%d Roll: %f Pitch: %f Yaw: %f\n\r", HAL_GetTick(), imu.rol, imu.pit, imu.yaw);
    if (!d_av && this->CANSystem->available(2, 0) || d_av) {
        d_av = true;
        accumulator->data(this->CANSystem->get(2, 0));
    }
    sprintf(buf, "%8d %8d", (int) accumulator->get_round(), (int) accumulator->get_overflow());
    this->debugSystem->toggle_led(3);
    this->debugSystem->info("OI", buf);

    HAL_Delay(100);
}
