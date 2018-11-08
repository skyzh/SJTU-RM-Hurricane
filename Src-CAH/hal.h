//
// Created by Alex Chi on 2018/11/02.
//

#ifndef HURRICANE_HAL_H
#define HURRICANE_HAL_H
extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "dma.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "../Src/runtime/bsp_imu.h"
#include "../Src/runtime/bsp_uart.h"
}

#endif //HURRICANE_HAL_H
