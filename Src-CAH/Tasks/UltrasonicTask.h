//
// Created by Alex Chi on 2018/11/20.
//

#ifndef HURRICANE_ULTRASONICTASK_H
#define HURRICANE_ULTRASONICTASK_H

#include "CAHRR/src/Task.h"
#include "CAHRR/src/PIDAccumulator.h"

class UltrasonicTask : public Task {
private:
    PIDDisplacementAccumulator *accumulator;
public:
    UltrasonicTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_ULTRASONICTASK_H
