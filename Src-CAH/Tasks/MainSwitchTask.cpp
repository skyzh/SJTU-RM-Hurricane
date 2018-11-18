//
// Created by Alex Chi on 2018/11/17.
//

#include "MainSwitchTask.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"

bool MainSwitchTask::when() {
    return oi->remoteSystem->getButton(CTR_SW1) == 1;
}

MainSwitchTask::MainSwitchTask(Task *task1, Task *task2) : ConditionSwitchTask(task1, task2) {

}

bool MainSwitchSwitchTask::when() {
    return oi->remoteSystem->getButton(CTR_SW1) == 2;
}

MainSwitchSwitchTask::MainSwitchSwitchTask(Task *task1, Task *task2) : ConditionSwitchTask(task1, task2) {

}
