//
// Created by Alex Chi on 2018/11/02.
//

#include <cstdio>
#include "hal.h"
#include "OI.h"

char buf[1000] = "Boot sequence ...";

OI *oi;
extern "C" void charr_bootstrap() {
    oi = new OI;
    oi->boostrap();
}

extern "C" void charr_loop() {
    oi->loop();
}


void OI::boostrap() {
    HAL_UART_Transmit(&huart2, (uint8_t *) buf, strlen(buf), 55);

    mpu_device_init();
    init_quaternion();

    std::sprintf(buf, " complete\n\r");
    HAL_UART_Transmit(&huart2, (uint8_t *) buf, strlen(buf), 55);
}

void OI::loop() {
    HAL_IWDG_Refresh(&hiwdg);
    HAL_GPIO_TogglePin(GPIOG, STAT8_Pin | STAT7_Pin | STAT6_Pin | STAT5_Pin
                              | STAT4_Pin | STAT3_Pin | STAT2_Pin | STAT1_Pin);

    mpu_get_data();
    imu_ahrs_update();
    imu_attitude_update();
    std::sprintf(buf, "%d Roll: %f Pitch: %f Yaw: %f\n\r", HAL_GetTick(), imu.rol, imu.pit, imu.yaw);
    HAL_UART_Transmit(&huart2, (uint8_t *) buf, strlen(buf), 100);
    HAL_Delay(10);
}
