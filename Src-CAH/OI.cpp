//
// Created by Alex Chi on 2018/11/02.
//

#include <cstdio>
#include "hal.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneCANSystem.h"
#include "HurricaneIMUSystem.h"
#include "HurricaneRemoteSystem.h"
#include "HurricaneArmSystem.h"
#include "HurricaneChassisSystem.h"
#include "HurricaneUltrasonicSystem.h"
#include "HurricaneCollectorSystem.h"
#include "HurricaneClawSystem.h"
#include "MainTask.h"
#include "CAHRR/src/Task.h"

OI *oi;

extern "C" void charr_bootstrap() {
    oi = new OI;
    oi->boostrap();
}

extern "C" void charr_loop() {
    oi->loop();
}

Task *mainTask();

void OI::boostrap() {
    // initialize sequence
    this->debugSystem = new HurricaneDebugSystem;
    this->CANSystem = new HurricaneCANSystem(0);
    this->IMUSystem = new HurricaneIMUSystem;
    this->remoteSystem = new HurricaneRemoteSystem;
    this->chassisSystem = new HurricaneChassisSystem;
    this->armSystem = new HurricaneArmSystem;
    this->usSystemChassis = new HurricaneUltrasonicSystem(&htim8, TIM_CHANNEL_1, TIM_CHANNEL_3);
    this->collectorSystem = new HurricaneCollectorSystem();
    this->clawSystem = new HurricaneClawSystem();
    this->initialized = true;

    OK(this->debugSystem->initialize());
    OK(this->debugSystem->info("OI", "---- booting sequence ----"));

    OK(this->CANSystem->initialize());
    OK(this->IMUSystem->initialize());
    OK(this->remoteSystem->initialize());

    OK(this->debugSystem->info("OI", "---- user application ----"));

    OK(this->chassisSystem->initialize());
    OK(this->armSystem->initialize());
    OK(this->collectorSystem->initialize());
    OK(this->clawSystem->initialize());

    OK(this->usSystemChassis->initialize());

    OK(this->debugSystem->info("OI", "--- system initialized ---"));

    HAL_Delay(10);

    this->task = mainTask();
    OK(this->task->initialize());
}

void OI::loop() {
    // update data source
    OK(this->debugSystem->alive());
    OK(this->remoteSystem->update());
    OK(this->IMUSystem->update());
    OK(this->usSystemChassis->update());

    // update tasks
    OK(this->task->update());

    // update user systems
    OK(this->chassisSystem->update());
    OK(this->armSystem->update());
    OK(this->collectorSystem->update());
    OK(this->clawSystem->update());

    // update data destination
    OK(this->CANSystem->update());
    HAL_Delay(10);
}

extern "C" void hurricane_error_handler(char *file, int line) {
    oi->debugSystem->error("ERR", std::string(file) + "@" + std::to_string(line));
}

extern "C" void hurricane_debug(const char *info) {
    oi->debugSystem->info("DBG", info);
}