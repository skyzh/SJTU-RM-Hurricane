//
// Created by Alex Chi on 2018/11/12.
//

#include <vector>

#include "OI.h"
#include "CAHRR/src/ParallelTask.h"
#include "Tasks/TankDriveTask.h"
#include "Tasks/MainSwitchTask.h"
#include "Tasks/GyroDriveTask.h"

Task *mainTask() {
    return new MainSwitchTask(new TankDriveTask, new GyroDriveTask);
}
