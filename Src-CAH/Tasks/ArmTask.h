//
// Created by Alex Chi on 2018/11/17.
//

#ifndef HURRICANE_ARMTASK_H
#define HURRICANE_ARMTASK_H

#include "CAHRR/src/Task.h"

class ArmTask : public Task {
public:
    ArmTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_ARMTASK_H
