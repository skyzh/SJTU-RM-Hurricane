//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANECHASSISSYSTEM_H
#define HURRICANE_HURRICANECHASSISSYSTEM_H

#include "CAHRR/src/ChassisSystem.h"

const int CHASSIS_FL_ID = 1;
const int CHASSIS_FR_ID = 2;
const int CHASSIS_BL_ID = 3;
const int CHASSIS_BR_ID = 4;

class HurricaneChassisSystem : public ChassisSystem {
private:

public:
    HurricaneChassisSystem();

    virtual bool initialize();

    virtual bool update();

    virtual bool destroy();

    virtual bool setSpeed(double bf, double lr);
};

#endif //HURRICANE_HURRICANECHASSISSYSTEM_H
