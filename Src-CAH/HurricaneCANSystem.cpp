//
// Created by Alex Chi on 2018/11/09.
//

#include <cstring>
#include "OI.h"
#include "HurricaneCANSystem.h"
#include "HurricaneDebugSystem.h"

#define CAN_SYSTEM_H0 hcan1
#define CAN_SYSTEM_H1 hcan2
#define CAN_SYSTEM_RX0 CAN_RX_FIFO0
#define CAN_SYSTEM_RX1 CAN_RX_FIFO1
#define CAN_SYSTEM_F0 CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY
#define CAN_SYSTEM_F1 CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY
#define CAN_SYSTEM_ID_BASE 0x200

__weak void HURRICANE_CAN0_2_DATA() {
}

HurricaneCANSystem::HurricaneCANSystem(int can_id) : can_id(can_id) {
}

bool HurricaneCANSystem::initialize() {
    OK(oi->debugSystem->info("CAN", "CAN " + std::to_string(can_id) + " initialize"));

    // initialize can handle
    CAN_HandleTypeDef *hcan = &(can_id == 0 ? CAN_SYSTEM_H0 : CAN_SYSTEM_H1);
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = (can_id == 0 ? CAN_SYSTEM_RX0 : CAN_SYSTEM_RX1);
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
    if (HAL_CAN_Start(hcan) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(hcan, (can_id == 0 ? CAN_SYSTEM_F0 : CAN_SYSTEM_F1)) != HAL_OK) {
        Error_Handler();
    }

    // initialize data storage
    memset(this->can_write_data, 0, sizeof(this->can_write_data));
    memset(this->can_recv_data, 0, sizeof(this->can_recv_data));
    memset(this->can_recv_data_available, 0, sizeof(this->can_recv_data_available));

    // intialize tx header
    this->TxHeader.StdId = CAN_SYSTEM_ID_BASE;
    this->TxHeader.ExtId = 0x01;
    this->TxHeader.RTR = CAN_RTR_DATA;
    this->TxHeader.IDE = CAN_ID_STD;
    this->TxHeader.DLC = 8;
    this->TxHeader.TransmitGlobalTime = DISABLE;

    OK(oi->debugSystem->info("CAN", "  ... complete"));
    return true;
}

bool HurricaneCANSystem::destroy() {
    return false;
}

bool HurricaneCANSystem::update() {
    CAN_HandleTypeDef *hcan = &(can_id == 0 ? CAN_SYSTEM_H0 : CAN_SYSTEM_H1);
    for (int i = 1; i <= 4; i++) {
        uint8_t _i = i - 1;
        this->TxData[_i << 1] = this->can_write_data[i] >> 8;
        this->TxData[(_i << 1) | 1] = this->can_write_data[i];
    }
    return HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox) == HAL_OK;
}

bool HurricaneCANSystem::set(int id, uint16_t data) {
    this->can_write_data[id] = data;
    return true;
}

bool HurricaneCANSystem::available(int id, int data_id) {
    return this->can_recv_data_available[id][data_id];
}

uint16_t HurricaneCANSystem::get(int id, int data_id) {
    return this->can_recv_data[id][data_id];
}

bool HurricaneCANSystem::data(int can_id) {
    CAN_HandleTypeDef *hcan = &(can_id == 0 ? CAN_SYSTEM_H0 : CAN_SYSTEM_H1);
    HAL_CAN_GetRxMessage(hcan, (can_id == 0 ? CAN_SYSTEM_RX0 : CAN_SYSTEM_RX1), &RxHeader, RxData);
    const uint32_t id = RxHeader.StdId - CAN_SYSTEM_ID_BASE;
    for (uint8_t i = 0; i < CAN_MAX_DATA_ID; i++) {
        this->can_recv_data[id][i] = (RxData[i << 1] << 8) | RxData[(i << 1) | 1];
        this->can_recv_data_available[id][i] = true;
    }
    HURRICANE_CAN0_2_DATA();
    return true;
}

extern "C" void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == &CAN_SYSTEM_H0) {
        oi->debugSystem->toggle_led(2);
    }
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == &CAN_SYSTEM_H0) {
        oi->CANSystem->data(0);
    }
}
