//
// Created by Alex Chi on 2018/11/10.
//

#include "HurricaneIMUSystem.h"
#include "HurricaneDebugSystem.h"
#include "OI.h"

HurricaneIMUSystem::HurricaneIMUSystem() {

}

bool HurricaneIMUSystem::initialize() {
    OK(oi->debugSystem->info("IMU", "MPU initialize"));
    mpu_device_init();
    init_quaternion();
    OK(oi->debugSystem->info("IMU", "  ... complete"));
    return true;
}

bool HurricaneIMUSystem::destroy() {
    return false;
}

bool HurricaneIMUSystem::update() {
    mpu_get_data();
    imu_ahrs_update();
    imu_attitude_update();
    return true;
}

float HurricaneIMUSystem::get(int data) {
    switch(data) {
        case IMU_ROL:
            return imu.rol;
        case IMU_PIT:
            return imu.pit;
        case IMU_YAW:
            return imu.yaw;
        default:
            return 0;
    }
}
