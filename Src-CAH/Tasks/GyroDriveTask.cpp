//
// Created by Alex Chi on 2018/11/17.
//

#include "GyroDriveTask.h"
#include "OI.h"
#include "HurricaneChassisSystem.h"
#include "HurricaneIMUSystem.h"
#include "HurricaneDebugSystem.h"
#include "HurricaneRemoteSystem.h"

#include "CAHRR/src/utils.h"

const double GYRO_DRIVE_OUTPUT_LIMIT = 0.5;

double Kp = 0.0118, Ki = 0.0002, Kd = 0.0038;

GyroDriveTask::GyroDriveTask() : Task() {

    this->accumulator = new PIDDisplacementAccumulator();
    this->accumulator->set_pid(Kp, Ki, Kd);
    this->accumulator->set_output(-GYRO_DRIVE_OUTPUT_LIMIT, GYRO_DRIVE_OUTPUT_LIMIT);
}

bool GyroDriveTask::initialize() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    this->accumulator->reset();
    return true;
}

bool GyroDriveTask::destroy() {
    oi->chassisSystem->setSpeed(0, 0, 0);
    return true;
}

bool GyroDriveTask::isEnd() {
    return false;
}

bool GyroDriveTask::update() {
    static char __buf[1000];
    const double target = 180;
    double output = clamp(this->accumulator->calc(cycle_err_double(target, oi->IMUSystem->get(IMU_YAW), -180.0, 180.0)), -GYRO_DRIVE_OUTPUT_LIMIT, GYRO_DRIVE_OUTPUT_LIMIT);

    HDEBUG_BEGIN(10)
            sprintf(__buf, "%f %f %f | %f | %f %f %f", oi->IMUSystem->get(IMU_PIT),
                    oi->IMUSystem->get(IMU_ROL),
                    oi->IMUSystem->get(IMU_YAW), output, Kp, Ki, Kd);
            oi->debugSystem->info("GYRO", __buf);
    HDEBUG_END()
/*
    HDEBUG_ONCE_BEGIN(oi->remoteSystem->getAxis(CTR_CH1) == 660)
        Kp += 0.0001;
        this->accumulator->set_pid(Kp, Ki, Kd);
        this->accumulator->reset();
    HDEBUG_ONCE_END()

    HDEBUG_ONCE_BEGIN(oi->remoteSystem->getAxis(CTR_CH1) == -660)
        Kp -= 0.0001;
        this->accumulator->set_pid(Kp, Ki, Kd);
        this->accumulator->reset();
    HDEBUG_ONCE_END()

    HDEBUG_ONCE_BEGIN(oi->remoteSystem->getAxis(CTR_CH2) == 660)
        Ki += 0.0001;
        this->accumulator->set_pid(Kp, Ki, Kd);
        this->accumulator->reset();
    HDEBUG_ONCE_END()

    HDEBUG_ONCE_BEGIN(oi->remoteSystem->getAxis(CTR_CH2) == -660)
        Ki -= 0.0001;
        this->accumulator->set_pid(Kp, Ki, Kd);
        this->accumulator->reset();
    HDEBUG_ONCE_END()

    HDEBUG_ONCE_BEGIN(oi->remoteSystem->getAxis(CTR_CH3) == 660)
        Kd += 0.0001;
        this->accumulator->set_pid(Kp, Ki, Kd);
        this->accumulator->reset();
    HDEBUG_ONCE_END()

    HDEBUG_ONCE_BEGIN(oi->remoteSystem->getAxis(CTR_CH3) == -660)
        Kd -= 0.0001;
        this->accumulator->set_pid(Kp, Ki, Kd);
        this->accumulator->reset();
    HDEBUG_ONCE_END()
*/
    oi->chassisSystem->setSpeed(0, 0, output);
    return true;
}
