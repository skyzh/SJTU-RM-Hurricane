//
// Created by Alex Chi on 2018/11/22.
//

#include "RightSwitchTask.h"
#include "OI.h"
#include "HurricaneRemoteSystem.h"

bool RightSwitchTask::trigger() {
    return oi->remoteSystem->getButton(CTR_SW2) == 2;
}

RightSwitchTask::RightSwitchTask(Task *task1, Task *task2) : TriggerSwitchTask(task1, task2) {

}
