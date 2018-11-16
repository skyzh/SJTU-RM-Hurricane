//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANECHASSISSYSTEM_H
#define HURRICANE_HURRICANECHASSISSYSTEM_H

#include "CAHRR/src/ChassisSystem.h"
#include "CAHRR/src/PIDAccumulator.h"

const int CHASSIS_FL_ID = 1;
const int CHASSIS_FR_ID = 2;
const int CHASSIS_BL_ID = 3;
const int CHASSIS_BR_ID = 4;

class HurricaneChassisSystem {
private:
    double bf, lr, rot;
    PIDAccumulator *fl, *fr, *bl, *br;
public:
    HurricaneChassisSystem();

    virtual bool initialize();

    virtual bool update();

    virtual bool destroy();

    virtual bool setSpeed(double bf, double lr, double rot);
};

#endif //HURRICANE_HURRICANECHASSISSYSTEM_H
