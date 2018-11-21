//
// Created by Alex Chi on 2018/11/22.
//

#ifndef HURRICANE_CLAWTASK_H
#define HURRICANE_CLAWTASK_H

#include "CAHRR/src/Task.h"

class ClawTask : public Task {
public:
    ClawTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_CLAWTASK_H
