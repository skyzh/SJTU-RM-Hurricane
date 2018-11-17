//
// Created by Alex Chi on 2018/11/17.
//

#ifndef HURRICANE_MAINSWITCHTASK_H
#define HURRICANE_MAINSWITCHTASK_H

#include "../CAHRR/src/ConditionSwitchTask.h"

class MainSwitchTask : public ConditionSwitchTask {
public:
    virtual bool when();

    MainSwitchTask(Task *task1, Task *task2);
};


#endif //HURRICANE_MAINSWITCHTASK_H
