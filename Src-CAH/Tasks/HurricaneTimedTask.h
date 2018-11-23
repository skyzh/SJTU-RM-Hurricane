//
// Created by Alex Chi on 2018/11/24.
//

#ifndef HURRICANE_HURRICANETIMEDTASK_H
#define HURRICANE_HURRICANETIMEDTASK_H

#include "CAHRR/src/TimedTask.h"
#include "hal.h"

class HurricaneTimedTask : public TimedTask {
public:
    HurricaneTimedTask(unsigned int total_time) : TimedTask(total_time) {
    }

    virtual unsigned int time() {
        return HAL_GetTick();
    }
};

#endif //HURRICANE_HURRICANETIMEDTASK_H
