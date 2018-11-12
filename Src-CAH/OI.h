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

class Task;

class OI{
public:
    void boostrap();
    void loop();

    HurricaneDebugSystem* debugSystem;
    HurricaneCANSystem* CANSystem;
    HurricaneIMUSystem* IMUSystem;
    HurricaneRemoteSystem* remoteSystem;
    HurricaneChassisSystem* chassisSystem;
    HurricaneArmSystem* armSystem;

    Task* task;
};

extern OI* oi;

#define OK(x) if (!(x)) Error_Handler()

#endif //HURRICANE_OI_H
