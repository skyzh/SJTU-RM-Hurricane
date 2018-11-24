//
// Created by Alex Chi on 2018/11/12.
//

#include <vector>

#include "OI.h"
#include "CAHRR/src/ParallelTask.h"
#include "Tasks/TankDriveTask.h"
#include "Tasks/MainSwitchTask.h"
#include "Tasks/SaveDriveTask.h"
#include "Tasks/ServoTask.h"
#include "Tasks/ArmTask.h"

Task *mainTask() {

    return new ParallelTask(std::vector<Task *>({
            new LeftSwitchNone(
                    nullptr,
                    new RightSwitchBottom(
                            new LeftSwitchBottom(
                                    new LeftSwitchTop(
                                            new TankDriveTask,
                                            nullptr
                                    ),
                                    new SaveDriveTask
                            ),
                            new ArmTask
                    )

            ),
            new RightSwitchTopTrigger(
                    new CollectorCloseTask,
                    new CollectorOpenTask)
    }));
}
