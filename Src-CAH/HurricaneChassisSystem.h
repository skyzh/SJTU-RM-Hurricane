//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANECHASSISSYSTEM_H
#define HURRICANE_HURRICANECHASSISSYSTEM_H

#include "CAHRR/src/ChassisSystem.h"
#include "CAHRR/src/PIDAccumulator.h"
#include "CAHRR/src/RampAccumulator.h"

class HurricaneChassisSystem {
private:
    double bf, lr, rot;
    PIDRateAccumulator *pid_acc[4];
    RampAccumulator <double> *ramp_acc[4];
public:
    bool disabled = true; // todo: remove
    const double output_limit = 12000.0;
    const int max_rpm = 5000;
    const double ramp_limit = 80;
    const double Kp = 3.0, Ki = 0, Kd = 1.5;

    const int CHASSIS_FL_ID = 2;
    const int CHASSIS_FR_ID = 1;
    const int CHASSIS_BL_ID = 3;
    const int CHASSIS_BR_ID = 4;

    HurricaneChassisSystem();

    virtual bool initialize();

    virtual bool update();

    virtual bool destroy();

    virtual bool setSpeed(double bf, double lr, double rot);
};

#endif //HURRICANE_HURRICANECHASSISSYSTEM_H
