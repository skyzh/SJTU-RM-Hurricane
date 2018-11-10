//
// Created by Alex Chi on 2018/11/10.
//

#ifndef HURRICANE_HURRICANEIMUSYSTEM_H
#define HURRICANE_HURRICANEIMUSYSTEM_H

#include "hal.h"
#include "CAHRR/src/IMUSystem.h"

#define IMU_ROL 0
#define IMU_PIT 1
#define IMU_YAW 2

class HurricaneIMUSystem {
public:
    HurricaneIMUSystem();

    bool initialize();

    bool destroy();

    bool update();

    float get(int data);
};

#endif //HURRICANE_HURRICANEIMUSYSTEM_H
