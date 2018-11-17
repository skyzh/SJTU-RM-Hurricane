//
// Created by Alex Chi on 2018/11/17.
//

#ifndef HURRICANE_TANKDRIVETASK_H
#define HURRICANE_TANKDRIVETASK_H

#include "../CAHRR/src/Task.h"

class TankDriveTask : public Task {
public:
    TankDriveTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_TANKDRIVETASK_H
