//
// Created by Alex Chi on 2018/11/09.
//

#ifndef HURRICANE_HURRICANECANSYSTEM_H
#define HURRICANE_HURRICANECANSYSTEM_H

#include "hal.h"
#include "CAHRR/Src/CANSystem.h"

const int CAN_MAX_CAN_ID = 10;
const int CAN_MAX_DATA_ID = 4;

class HurricaneCANSystem : public CANSystem {
private:
    uint16_t can_write_data[CAN_MAX_CAN_ID];
    uint16_t can_recv_data[CAN_MAX_CAN_ID][CAN_MAX_DATA_ID];
    bool can_recv_data_available[CAN_MAX_CAN_ID][CAN_MAX_DATA_ID];

    CAN_FilterTypeDef sFilterConfig;
    CAN_TxHeaderTypeDef TxHeader;
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    uint8_t TxData[8];
    uint32_t TxMailbox;

public:
    HurricaneCANSystem();

    virtual bool initialize();

    virtual bool initialize(int can_id);

    virtual bool destroy();

    virtual bool update();

    virtual bool set(int id, uint16_t data);

    virtual uint16_t get(int id, int data_id);

    virtual bool available(int id, int data_id);

    bool data(int can_id);
};


#endif //HURRICANE_HURRICANECANSYSTEM_H
