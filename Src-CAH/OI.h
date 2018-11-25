//
// Created by Alex Chi on 2018/11/02.
//

#ifndef HURRICANE_OI_H
#define HURRICANE_OI_H

class HurricaneDebugSystem;

class HurricaneCANSystem;

class HurricaneIMUSystem;

class HurricaneRemoteSystem;

class HurricaneChassisSystem;

class HurricaneArmSystem;

class HurricaneCollectorSystem;

class HurricaneClawSystem;

class Task;

class OI {
public:
    OI() : initialized(false) {}

    void boostrap();

    void loop();

    HurricaneDebugSystem *debugSystem;
    HurricaneCANSystem *CANSystem;
    HurricaneIMUSystem *IMUSystem;
    HurricaneRemoteSystem *remoteSystem;
    HurricaneChassisSystem *chassisSystem;
    HurricaneArmSystem *armSystem;
    HurricaneCollectorSystem *collectorSystem;
    HurricaneClawSystem *clawSystem;
    Task *task;

    bool initialized;
};

extern OI *oi;

#define OK(x) if (!(x)) Error_Handler()
#define HSYS(sys) if (oi && oi->initialized && oi->sys)
#endif //HURRICANE_OI_H
