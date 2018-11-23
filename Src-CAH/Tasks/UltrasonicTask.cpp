//
// Created by Alex Chi on 2018/11/20.
//

#include "UltrasonicTask.h"
#include "hal.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneUltrasonicSystem.h"
#include "HurricaneChassisSystem.h"
#include "CAHRR/src/utils.h"

UltrasonicTask::UltrasonicTask() : Task() {
    this->accumulator = new PIDDisplacementAccumulator();
    this->accumulator->set_pid(1.0, 0.0, 0.0);
    this->accumulator->set_output(-0.1, 0.1);
}

bool UltrasonicTask::initialize() {
    this->accumulator->reset();
    return true;
}

bool UltrasonicTask::destroy() {
    return true;
}

bool UltrasonicTask::isEnd() {
    return true;
}

bool UltrasonicTask::update() {
    HDEBUG_BEGIN(10)
    static char _buf[1000];
    sprintf(_buf, "uts1 %f uts2 %f", oi->usSystemChassis->distance(0), oi->usSystemChassis->distance(1));
            oi->debugSystem->info("UTS", _buf);
    HDEBUG_END()
    return true;
}

