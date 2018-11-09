//
// Created by Alex Chi on 2018/11/02.
//

#ifndef HURRICANE_OI_H
#define HURRICANE_OI_H

class HurricaneDebugSystem;

class OI{
public:
    void boostrap();
    void loop();

    HurricaneDebugSystem* debugSystem;
};

extern OI* oi;

#define OK(x) ((x) == true)

#endif //HURRICANE_OI_H
