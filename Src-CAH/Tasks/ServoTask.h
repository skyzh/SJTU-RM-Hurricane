//
// Created by Alex Chi on 2018/11/19.
//

#ifndef HURRICANE_SERVOTASK_H
#define HURRICANE_SERVOTASK_H

#include "CAHRR/src/Task.h"

class ServoTask : public Task {
public:
    ServoTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_SERVOTASK_H
