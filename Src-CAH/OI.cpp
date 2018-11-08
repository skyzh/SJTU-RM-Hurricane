//
// Created by Alex Chi on 2018/11/02.
//

#include <cstdio>
#include "hal.h"
#include "OI.h"

char buf[1000] = "Boot sequence ...";

OI *oi;
extern "C" void charr_bootstrap() {
    oi = new OI;
    oi->boostrap();
}

extern "C" void charr_loop() {
    oi->loop();
}

typedef struct
{
    uint8_t Data[8];
} CAN_RecvMsg;


CAN_FilterTypeDef sFilterConfig;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;

extern "C" void hurricane_can_recv_can1() {

}

extern "C" void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
    HAL_GPIO_TogglePin(GPIOG, STAT2_Pin);
}
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
    HAL_GPIO_TogglePin(GPIOG, STAT2_Pin);
}

void init_can() {
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
    if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        Error_Handler();
    }

    if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK){
        Error_Handler();
    }

}

void OI::boostrap() {
    HAL_UART_Transmit(&huart2, (uint8_t *) buf, strlen(buf), 55);

    mpu_device_init();
    init_quaternion();
    init_can();

    dbus_uart_init();

    std::sprintf(buf, " complete\n\r");
    HAL_UART_Transmit(&huart2, (uint8_t *) buf, strlen(buf), 55);
}

extern rc_info_t rc;

void OI::loop() {
    HAL_IWDG_Refresh(&hiwdg);
    mpu_get_data();
    imu_ahrs_update();
    imu_attitude_update();
    // std::sprintf(buf, "%d Roll: %f Pitch: %f Yaw: %f\n\r", HAL_GetTick(), imu.rol, imu.pit, imu.yaw);
    sprintf(buf, "%8d %8d\n\r", RxData[0] << 8 | RxData[1], RxData[2] << 8 | RxData[3]);
    if (rc.mx) HAL_GPIO_TogglePin(GPIOG, STAT8_Pin);
    if (rc.my) HAL_GPIO_TogglePin(GPIOG, STAT7_Pin);
    if (rc.mz) HAL_GPIO_TogglePin(GPIOG, STAT6_Pin);
    if (rc.ml) HAL_GPIO_TogglePin(GPIOG, STAT5_Pin);
    if (rc.mr) HAL_GPIO_TogglePin(GPIOG, STAT4_Pin);
    if (rc.kbd) HAL_GPIO_TogglePin(GPIOG, STAT3_Pin);

    HAL_UART_Transmit(&huart2, (uint8_t *) buf, strlen(buf), 1000);

    HAL_Delay(100);
}

