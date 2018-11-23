//
// Created by Alex Chi on 2018/11/12.
//

#include <vector>

#include "OI.h"
#include "CAHRR/src/ParallelTask.h"
#include "Tasks/TankDriveTask.h"
#include "Tasks/MainSwitchTask.h"
#include "Tasks/ArmTask.h"
#include "Tasks/GyroDriveTask.h"
#include "Tasks/ServoTask.h"
#include "Tasks/UltrasonicTask.h"
#include "Tasks/ClawTask.h"
#include "Tasks/RightSwitchTask.h"
#include "Tasks/ArmDisableTask.h"

Task *mainTask() {

    return new ParallelTask(std::vector<Task *>({new MainSwitchSwitchTask(
            new MainSwitchTask(
                    // CTL_SW1 = 2
                    new TankDriveTask,
                    // CTL_SW1 = 1
                    new GyroDriveTask),
            // CTL_SW1 = 3
            new UltrasonicTask), new RightSwitchTask(new ArmDisableTask, nullptr)
                                                }));
}
