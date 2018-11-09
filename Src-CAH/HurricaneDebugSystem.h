//
// Created by Alex Chi on 2018/11/09.
//

#ifndef HURRICANE_HURRICANEDEBUGSYSTEM_H
#define HURRICANE_HURRICANEDEBUGSYSTEM_H

#include "CAHRR/src/DebugSystem.h"
#include <queue>

class HurricaneDebugSystem : public DebugSystem {
private:
    bool transmit(const std::string &data);

    bool transmission_in_progress;
    std::queue<std::string> messages;
public:
    HurricaneDebugSystem();

    virtual bool initialize();

    virtual bool error(const char *&src, const char *&message);

    virtual bool info(const char *&src, const char *&message);

    virtual bool error(const std::string &src, const std::string &message);

    virtual bool info(const std::string &src, const std::string &message);

    virtual bool set_led(int id, bool val);

    virtual bool toggle_led(int id);

    virtual bool alive();

    virtual bool destroy();

    bool next_transmission();
};

#define DEBUG_UART huart2

#endif //HURRICANE_HURRICANEDEBUGSYSTEM_H
