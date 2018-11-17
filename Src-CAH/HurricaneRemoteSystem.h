//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANEREMOTESYSTEM_H
#define HURRICANE_HURRICANEREMOTESYSTEM_H

#include "hal.h"
#include "CAHRR/src/RemoteSystem.h"

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

class HurricaneRemoteSystem : public RemoteSystem {
public:
    HurricaneRemoteSystem();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool update();

    virtual int16_t getAxis(int id);

    virtual int16_t getButton(int id);
};


#endif //HURRICANE_HURRICANEREMOTESYSTEM_H
