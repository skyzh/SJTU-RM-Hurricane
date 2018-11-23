//
// Created by Alex Chi on 2018/11/24.
//

#ifndef HURRICANE_SAVEDRIVETASK_H
#define HURRICANE_SAVEDRIVETASK_H

#include "CAHRR/src/Task.h"

class SaveDriveTask : public Task {
public:
    SaveDriveTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();
};


#endif //HURRICANE_SAVEDRIVETASK_H
