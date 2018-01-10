#ifndef DECORADOR_H
#define DECORADOR_H

#include <iostream>
#include "Task.h"
#include "Blackboard.h"

class Decorador: public Task{
public:
    Decorador();
    virtual ~Decorador();
    virtual bool run(Blackboard* bb);
    bool terminate();
    void setChild(Task* newChild);
protected:
    Task* child;
};

#endif