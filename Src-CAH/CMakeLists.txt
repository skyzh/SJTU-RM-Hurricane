cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(CAHRR)

add_library(Hurricane OI.cpp HurricaneDebugSystem.cpp HurricaneDebugSystem.h HurricaneCANSystem.cpp HurricaneCANSystem.h ARMPIDAccumulator.cpp ARMPIDAccumulator.h HurricaneIMUSystem.cpp HurricaneIMUSystem.h HurricaneRemoteSystem.cpp HurricaneRemoteSystem.h MainTask.cpp MainTask.h HurricaneChassisSystem.cpp HurricaneChassisSystem.h HurricaneArmSystem.cpp HurricaneArmSystem.h Tasks/TestTask.cpp Tasks/TestTask.h HurricaneGPIOSystem.cpp HurricaneUltrasonicSystem.cpp HurricaneUltrasonicSystem.h)
