//
// Created by Alex Chi on 2018/11/24.
//

#ifndef HURRICANE_SERVOTASK_H
#define HURRICANE_SERVOTASK_H

#include "CAHRR/src/Task.h"

class CollectorOpenTask : public Task {
public:
    virtual bool initialize();

    virtual bool destroy();

    virtual bool update();

};

class CollectorCloseTask : public Task {
public:
    virtual bool initialize();

    virtual bool destroy();

    virtual bool update();

};

#endif //HURRICANE_SERVOTASK_H
