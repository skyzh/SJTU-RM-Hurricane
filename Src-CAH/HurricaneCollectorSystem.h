//
// Created by Alex Chi on 2018/11/21.
//

#ifndef HURRICANE_HURRICANECOLLECTORSYSTEM_H
#define HURRICANE_HURRICANECOLLECTORSYSTEM_H


class HurricaneCollectorSystem {
private:
    double position;
public:
    HurricaneCollectorSystem();

    bool initialize();

    bool update();

    bool destroy();

    bool setPosition(double position);
};


#endif //HURRICANE_HURRICANECOLLECTORSYSTEM_H
