//
// Created by Alex Chi on 2018/11/13.
//

#include "HurricaneUltrasonicSystem.h"
#include "OI.h"
#include "HurricaneDebugSystem.h"

const int UTS_STATUS_PENDING = 0;
const int UTS_STATUS_SENT = 1;
const int UTS_STATUS_WAIT = 2;
const int UTS_STATUS_ECHO = 3;

void HurricaneUltrasonicSystem::send_data() {
    if (this->status == UTS_STATUS_PENDING) {
        oi->debugSystem->info("UTS", "sent");
        this->status = UTS_STATUS_SENT;
        this->lst_sent = HAL_GetTick();
        HAL_GPIO_WritePin(this->gpio_trig, this->trig_pin, GPIO_PIN_SET);
    } else if (this->status == UTS_STATUS_SENT) {
        if (HAL_GetTick() - this->lst_sent >= 20) {
            oi->debugSystem->info("UTS", "end");
            this->status = UTS_STATUS_WAIT;
            HAL_GPIO_WritePin(this->gpio_trig, this->trig_pin, GPIO_PIN_RESET);
        }
    }
}

HurricaneUltrasonicSystem::HurricaneUltrasonicSystem(GPIO_TypeDef *gpio_trig, uint16_t trig_pin,
                                                     GPIO_TypeDef *gpio_echo, uint16_t echo_pin) :
        gpio_trig(gpio_trig),
        gpio_echo(gpio_echo),
        trig_pin(trig_pin),
        echo_pin(echo_pin),
        lst_trigger_up(0),
        final_delta_time(0),
        status(UTS_STATUS_PENDING),
        lst_sent(0) {
    this->accumulator.reset();
}

bool HurricaneUltrasonicSystem::trigger() {
    oi->debugSystem->info("UTS", "triggered");
    if (this->status == UTS_STATUS_WAIT) {
        if (HAL_GPIO_ReadPin(this->gpio_echo, this->echo_pin) == GPIO_PIN_RESET) {
            this->lst_trigger_up = HAL_GetTick();
            this->status = UTS_STATUS_ECHO;
        }
    } else if (this->status == UTS_STATUS_ECHO) {
        if (HAL_GPIO_ReadPin(this->gpio_echo, this->echo_pin) == GPIO_PIN_SET) {
            this->accumulator.data(HAL_GetTick() - this->lst_trigger_up);
            this->status = UTS_STATUS_PENDING;
        }
    }
    return true;
}

bool HurricaneUltrasonicSystem::initialize() {
    oi->debugSystem->info("UTS", "ultrasonic system initialize");
    this->accumulator.reset();
    this->status = UTS_STATUS_PENDING;
    this->lst_sent = HAL_GetTick();
    HAL_GPIO_WritePin(this->gpio_trig, this->trig_pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    oi->debugSystem->info("UTS", "  ... complete");
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
    this->send_data();
    if (this->accumulator.n == 0) {
    } else {
        oi->debugSystem->info("UTS", std::to_string(this->accumulator.sum()));
        this->final_delta_time = this->accumulator.sum();
    }
    return true;
}

uint16_t HurricaneUltrasonicSystem::time() {
    return this->final_delta_time;
}

