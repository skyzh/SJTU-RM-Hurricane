cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(CAHRR)

add_library(Hurricane OI.cpp HurricaneDebugSystem.cpp HurricaneDebugSystem.h HurricaneCANSystem.cpp HurricaneCANSystem.h HurricaneIMUSystem.cpp HurricaneIMUSystem.h HurricaneRemoteSystem.cpp HurricaneRemoteSystem.h MainTask.cpp MainTask.h HurricaneChassisSystem.cpp HurricaneChassisSystem.h HurricaneArmSystem.cpp HurricaneArmSystem.h HurricaneGPIOSystem.cpp HurricaneUltrasonicSystem.cpp HurricaneUltrasonicSystem.h Tasks/TankDriveTask.cpp Tasks/TankDriveTask.h Tasks/MainSwitchTask.cpp Tasks/MainSwitchTask.h HurricaneCollectorSystem.cpp HurricaneCollectorSystem.h Models/Arm.cpp Models/Arm.h Models/ArmBottom.cpp Models/ArmBottom.h Models/ArmTop.cpp Models/ArmTop.h Models/ArmBase.cpp Models/ArmBase.h HurricaneClawSystem.cpp HurricaneClawSystem.h Tasks/HurricaneTimedTask.cpp Tasks/HurricaneTimedTask.h Tasks/SaveDriveTask.cpp Tasks/SaveDriveTask.h Tasks/ServoTask.cpp Tasks/ServoTask.h Tasks/ArmTask.cpp Tasks/ArmTask.h)
