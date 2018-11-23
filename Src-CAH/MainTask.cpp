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
#include "Tasks/ClawTask.h"

Task *mainTask() {

    return new ParallelTask(std::vector<Task *>({
            new LeftSwitchNone(
                    nullptr,
                    new LeftSwitchBottom(
                            new LeftSwitchTop(
                                    new TankDriveTask,
                                    nullptr
                            ),
                            new SaveDriveTask
                    )
            ),
            new RightSwitchTopTrigger(
                    new CollectorCloseTask,
                    new CollectorOpenTask),
            new RightSwitchBottom(
                    new ClawOpenTask,
                    new ClawCloseTask
                    )
    }));
}
