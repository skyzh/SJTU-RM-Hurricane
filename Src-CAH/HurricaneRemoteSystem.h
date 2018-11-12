//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_HURRICANEREMOTESYSTEM_H
#define HURRICANE_HURRICANEREMOTESYSTEM_H

#include "hal.h"
#include "CAHRR/src/RemoteSystem.h"

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
