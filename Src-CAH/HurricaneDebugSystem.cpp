//
// Created by Alex Chi on 2018/11/09.
//

#include "hal.h"
#include "HurricaneDebugSystem.h"
#include "OI.h"

HurricaneDebugSystem::HurricaneDebugSystem() : DebugSystem() {
}

bool HurricaneDebugSystem::initialize() {
    this->transmission_in_progress = false;
    return this->info("DBG", "connection established");
}

bool HurricaneDebugSystem::error(const char *&src, const char *&message) {
    return this->error(std::string(src), std::string(message));
}

bool HurricaneDebugSystem::info(const char *&src, const char *&message) {
    return this->info(std::string(src), std::string(message));
}

bool HurricaneDebugSystem::error(const std::string &src, const std::string &message) {
    return this->transmit("[" + src + "]" + std::to_string(HAL_GetTick()) + "[E] " + message + "\n\r");
}

bool HurricaneDebugSystem::info(const std::string &src, const std::string &message) {
    return this->transmit("[" + src + "]" + std::to_string(HAL_GetTick()) + "[I] " + message + "\n\r");
}

bool HurricaneDebugSystem::set_led(int id, bool val) {
    return false;
}

bool HurricaneDebugSystem::toggle_led(int id) {
    switch (id) {
        case 1:
            HAL_GPIO_TogglePin(STAT1_GPIO_Port, STAT1_Pin);
            break;
        case 2:
            HAL_GPIO_TogglePin(STAT2_GPIO_Port, STAT2_Pin);
            break;
        case 3:
            HAL_GPIO_TogglePin(STAT3_GPIO_Port, STAT3_Pin);
            break;
        case 4:
            HAL_GPIO_TogglePin(STAT4_GPIO_Port, STAT4_Pin);
            break;
        case 5:
            HAL_GPIO_TogglePin(STAT5_GPIO_Port, STAT5_Pin);
            break;
        case 6:
            HAL_GPIO_TogglePin(STAT6_GPIO_Port, STAT6_Pin);
            break;
        case 7:
            HAL_GPIO_TogglePin(STAT7_GPIO_Port, STAT7_Pin);
            break;
        case 8:
            HAL_GPIO_TogglePin(STAT8_GPIO_Port, STAT8_Pin);
            break;
        default:
            break;
    }
    return false;
}

bool HurricaneDebugSystem::alive() {
    return false;
}

bool HurricaneDebugSystem::destroy() {
    return this->info("DBG", "system halted");
}

bool HurricaneDebugSystem::transmit(const std::string &data) {
    this->messages.push(data);
    if (this->transmission_in_progress) {
        return true;
    } else {
        return this->next_transmission();
    }
}

bool HurricaneDebugSystem::next_transmission() {
    if (!this->messages.empty()) {
        this->transmission_in_progress = true;
        static std::string message;
        message = this->messages.front();
        this->messages.pop();
        return HAL_UART_Transmit_IT(&huart2, (uint8_t *) message.c_str(), (uint16_t) message.length()) == HAL_OK;
    } else {
        this->transmission_in_progress = false;
    }
    return true;
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &DEBUG_UART) {
        if (huart->TxXferCount == 0) {
            if (oi->debugSystem) oi->debugSystem->next_transmission();
        }
    }
}
