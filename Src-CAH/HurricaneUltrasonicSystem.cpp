//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"

void HurricaneUltrasonicSystem::send_data() {
    this->lst_sent = HAL_GetTick();
    HAL_GPIO_WritePin(this->gpio, this->write_pin, GPIO_PIN_SET);
}

HurricaneUltrasonicSystem::HurricaneUltrasonicSystem(GPIO_TypeDef *gpio, uint16_t write_pin) : gpio(gpio),
                                                                                               write_pin(write_pin),
                                                                                               lst_sent(0),
                                                                                               final_delta_time(0) {
    this->accumulator.reset();
}

bool HurricaneUltrasonicSystem::trigger() {
    this->accumulator.data(HAL_GetTick() - this->lst_sent);
    this->send_data();
    return true;
}

bool HurricaneUltrasonicSystem::initialize() {
    this->send_data();
    this->accumulator.reset();
    return true;
}

bool HurricaneUltrasonicSystem::destroy() {
    this->accumulator.reset();
    return true;
}

double HurricaneUltrasonicSystem::distance() {
    // TODO: calculate distance
    return this->final_delta_time * 340.0 / 1000.0;
}

bool HurricaneUltrasonicSystem::update() {
    if (this->accumulator.n == 0) {
        this->final_delta_time = 0;
        this->send_data();
    } else {
        this->final_delta_time = this->accumulator.sum();
    }
    return true;
}

