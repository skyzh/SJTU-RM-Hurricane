//
// Created by Alex Chi on 2018/11/22.
//

#ifndef HURRICANE_ARMDISABLETASK_H
#define HURRICANE_ARMDISABLETASK_H

#include "CAHRR/src/Task.h"

class ArmDisableTask : public Task {
public:
    ArmDisableTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_ARMDISABLETASK_H
