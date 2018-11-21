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
    //oi->debugSystem->info("UTS", std::to_string(oi->usSystemChassis->time() / 1000.0 / 1000.0 * 340.0 / 2 * 100));
    HDEBUG_END()
    return true;
}

