//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANEARMSYSTEM_H
#define HURRICANE_HURRICANEARMSYSTEM_H


class HurricaneArmSystem{
public:
    HurricaneArmSystem();

    bool initialize();

    bool update();

    bool destroy();

    bool setPosition(double location);
};


#endif //HURRICANE_HURRICANEARMSYSTEM_H
