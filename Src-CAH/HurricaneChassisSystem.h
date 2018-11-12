//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANECHASSISSYSTEM_H
#define HURRICANE_HURRICANECHASSISSYSTEM_H

#include "CAHRR/src/ChassisSystem.h"

class HurricaneChassisSystem : public ChassisSystem {
public:
    HurricaneChassisSystem();

    virtual bool initialize();

    virtual bool update();

    virtual bool destroy();

    virtual bool setSpeed(double bf, double lr);
};


#endif //HURRICANE_HURRICANECHASSISSYSTEM_H
