//
// Created by Alex Chi on 2018/11/22.
//

#ifndef HURRICANE_RIGHTSWITCHTASK_H
#define HURRICANE_RIGHTSWITCHTASK_H

#include "CAHRR/src/TriggerSwitchTask.h"

class RightSwitchTask : public TriggerSwitchTask {
public:
    RightSwitchTask(Task *task1, Task *task2);

    virtual bool trigger();
};


#endif //HURRICANE_RIGHTSWITCHTASK_H
