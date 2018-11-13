//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_TESTTASK_H
#define HURRICANE_TESTTASK_H

#include "CAHRR/src/Task.h"

class TestTask : public Task {
public:
    virtual bool initialize();

    virtual bool destroy();

    virtual bool update();
};


#endif //HURRICANE_TESTTASK_H
