//
// Created by Alex Chi on 2018/11/24.
//

#ifndef HURRICANE_CLAWTASK_H
#define HURRICANE_CLAWTASK_H

#include "CAHRR/src/Task.h"

class ClawOpenTask : public Task {
public:
    virtual bool initialize();

    virtual bool destroy();

    virtual bool update();
};

class ClawCloseTask : public Task {
public:
    virtual bool initialize();

    virtual bool destroy();

    virtual bool update();
};


#endif //HURRICANE_CLAWTASK_H
