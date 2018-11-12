//
// Created by Alex Chi on 2018/11/12.
//

#include "HurricaneRemoteSystem.h"
#include "oi.h"
#include "HurricaneDebugSystem.h"

extern rc_info_t rc;

#define CTR_CH1 0
#define CTR_CH2 1
#define CTR_CH3 2
#define CTR_CH4 3
#define CTR_SW1 4
#define CTR_SW2 5
#define CTR_MX 6
#define CTR_MY 7
#define CTR_MZ 8
#define CTR_ML 9
#define CTR_MR 10
#define CTR_KBD_W 11
#define CTR_KBD_S 12
#define CTR_KBD_A 13
#define CTR_KBD_D 14
#define CTR_KBD_Q 15
#define CTR_KBD_E 16
#define CTR_KBD_SFT 17
#define CTR_KBD_CTR 18

HurricaneRemoteSystem::HurricaneRemoteSystem() : RemoteSystem() {

}

bool HurricaneRemoteSystem::initialize() {
    OK(oi->debugSystem->info("RMT", "remote control initialize"));
    dbus_uart_init();
    OK(oi->debugSystem->info("RMT", "   ... complete"));
    return true;
}

bool HurricaneRemoteSystem::destroy() {
    return true;
}

int16_t HurricaneRemoteSystem::getAxis(int id) {
    switch(id) {
        case CTR_CH1:
            return rc.ch1;
        case CTR_CH2:
            return rc.ch2;
        case CTR_CH3:
            return rc.ch3;
        case CTR_CH4:
            return rc.ch4;
        case CTR_MX:
            return rc.mx;
        case CTR_MY:
            return rc.my;
        case CTR_MZ:
            return rc.mz;
        default:
            return 0;
    }
}

int16_t HurricaneRemoteSystem::getButton(int id) {
    switch(id) {
        case CTR_SW1:
            return rc.sw1;
        case CTR_SW2:
            return rc.sw2;
        case CTR_ML:
            return rc.ml;
        case CTR_MR:
            return rc.mr;
        case CTR_KBD_W:
            return rc.kbd & 1;
        case CTR_KBD_S:
            return rc.kbd & (1 << 1);
        case CTR_KBD_A:
            return rc.kbd & (1 << 2);
        case CTR_KBD_D:
            return rc.kbd & (1 << 3);
        case CTR_KBD_Q:
            return rc.kbd & (1 << 4);
        case CTR_KBD_E:
            return rc.kbd & (1 << 5);
        case CTR_KBD_SFT:
            return rc.kbd & (1 << 6);
        case CTR_KBD_CTR:
            return rc.kbd & (1 << 7);
        default:
            return 0;
    }
}

bool HurricaneRemoteSystem::update() {
    return true;
}
