//
// Created by Alex Chi on 2018/11/12.
//

#ifndef HURRICANE_MAINTASK_H
#define HURRICANE_MAINTASK_H

#include "CAHRR/src/Task.h"
class MainTask : public Task {
public:
    MainTask();

    virtual bool initialize();

    virtual bool destroy();

    virtual bool isEnd();

    virtual bool update();

    virtual ~MainTask() = default;
};


#endif //HURRICANE_MAINTASK_H
