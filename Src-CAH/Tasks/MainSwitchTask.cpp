//
// Created by Alex Chi on 2018/11/17.
//

#include "MainSwitchTask.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"

bool LeftSwitchTop::when() {
    return oi->remoteSystem->getButton(CTR_SW1) == 1;
}

LeftSwitchTop::LeftSwitchTop(Task *task1, Task *task2) : ConditionSwitchTask(task1, task2) {

}

bool LeftSwitchBottom::when() {
    return oi->remoteSystem->getButton(CTR_SW1) == 2;
}

LeftSwitchBottom::LeftSwitchBottom(Task *task1, Task *task2) : ConditionSwitchTask(task1, task2) {

}

bool LeftSwitchNone::when() {
    return oi->remoteSystem->getButton(CTR_SW1) != 0;
}

LeftSwitchNone::LeftSwitchNone(Task *task1, Task *task2) : ConditionSwitchTask(task1, task2) {

}

bool RightSwitchBottom::when() {
    return oi->remoteSystem->getButton(CTR_SW2) == 2;
}

RightSwitchBottom::RightSwitchBottom(Task *task1, Task *task2) : ConditionSwitchTask(task1, task2) {

}

RightSwitchTopTrigger::RightSwitchTopTrigger(Task *task1, Task *task2) : TriggerSwitchTask(task1, task2) {

}

bool RightSwitchTopTrigger::trigger() {
    return oi->remoteSystem->getButton(CTR_SW1) == 1;
}
