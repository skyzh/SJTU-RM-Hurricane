//
// Created by Alex Chi on 2018/11/17.
//

#ifndef HURRICANE_GYRODRIVETASK_H
#define HURRICANE_GYRODRIVETASK_H

#include "CAHRR/src/Task.h"
#include "CAHRR/src/PIDAccumulator.h"

class GyroDriveTask : public Task {
private:
    PIDDisplacementAccumulator *accumulator;
public:
    GyroDriveTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_GYRODRIVETASK_H
