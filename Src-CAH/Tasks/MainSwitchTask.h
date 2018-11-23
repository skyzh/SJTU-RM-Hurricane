//
// Created by Alex Chi on 2018/11/17.
//

#ifndef HURRICANE_MAINSWITCHTASK_H
#define HURRICANE_MAINSWITCHTASK_H

#include "CAHRR/src/ConditionSwitchTask.h"
#include "CAHRR/src/TriggerSwitchTask.h"

class LeftSwitchTop : public ConditionSwitchTask {
public:
    virtual bool when();

    LeftSwitchTop(Task *task1, Task *task2);
};

class LeftSwitchBottom : public ConditionSwitchTask {
public:
    virtual bool when();

    LeftSwitchBottom(Task *task1, Task *task2);
};

class LeftSwitchNone : public ConditionSwitchTask {
public:
    virtual bool when();

    LeftSwitchNone(Task *task1, Task *task2);
};


class RightSwitchBottom : public ConditionSwitchTask {
public:
    virtual bool when();

    RightSwitchBottom(Task *task1, Task *task2);
};

class RightSwitchTopTrigger : public TriggerSwitchTask {
public:
    RightSwitchTopTrigger(Task *task1, Task *task2);

    virtual bool trigger();
};
#endif //HURRICANE_MAINSWITCHTASK_H
