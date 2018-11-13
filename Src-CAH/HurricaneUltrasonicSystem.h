//
// Created by Alex Chi on 2018/11/13.
//

#ifndef HURRICANE_HURRICANEULTRASONICSYSTEM_H
#define HURRICANE_HURRICANEULTRASONICSYSTEM_H

#include "hal.h"
#include "CAHRR/src/UltrasonicSystem.h"
#include "CAHRR/src/AvgAccumulator.h"

class HurricaneUltrasonicSystem : public UltrasonicSystem {
private:
    uint16_t write_pin;
    uint32_t lst_sent;
    GPIO_TypeDef *gpio;
    AvgAccumulator <long long> accumulator;
    uint16_t final_delta_time;
    void send_data();

public:
    HurricaneUltrasonicSystem(GPIO_TypeDef *gpio, uint16_t write_pin);

    bool trigger();

    bool update();

    bool initialize();

    bool destroy();

    double distance();

    uint16_t time();
};


#endif //HURRICANE_HURRICANEULTRASONICSYSTEM_H
