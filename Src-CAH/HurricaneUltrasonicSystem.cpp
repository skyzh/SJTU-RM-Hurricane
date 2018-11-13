//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"

void HurricaneUltrasonicSystem::send_data() {
    HAL_GPIO_WritePin(this->gpio, this->write_pin, GPIO_PIN_SET);
}

HurricaneUltrasonicSystem::HurricaneUltrasonicSystem(GPIO_TypeDef *gpio, uint16_t read_pin, uint16_t write_pin) :
        gpio(gpio),
        read_pin(read_pin),
        write_pin(write_pin),
        lst_trigger_up(0),
        final_delta_time(0),
        fail_count(0) {
    this->accumulator.reset();
}

bool HurricaneUltrasonicSystem::trigger() {
    if (HAL_GPIO_ReadPin(this->gpio, this->read_pin) == GPIO_PIN_SET) {
        this->lst_trigger_up = HAL_GetTick();
    } else {
        this->accumulator.data(HAL_GetTick() - this->lst_trigger_up);
    }
    return true;
}

bool HurricaneUltrasonicSystem::initialize() {
    this->send_data();
    this->accumulator.reset();
    this->fail_count = 0;
    return true;
}

bool HurricaneUltrasonicSystem::destroy() {
    this->accumulator.reset();
    this->final_delta_time = 0;
    return true;
}

double HurricaneUltrasonicSystem::distance() {
    // TODO: calculate exact distance
    return this->final_delta_time * 340.0 / 1000.0;
}

bool HurricaneUltrasonicSystem::update() {
    if (this->accumulator.n == 0) {
        this->final_delta_time = 0;
        this->fail_count = this->fail_count + 1;
        if (this->fail_count >= 5) {
            this->fail_count = 0;
            this->send_data();
        }
        oi->debugSystem->error("UTS", "data failure");
    } else {
        this->fail_count = 0;
        this->final_delta_time = this->accumulator.sum();
    }
    return true;
}

uint16_t HurricaneUltrasonicSystem::time() {
    return this->final_delta_time;
}

