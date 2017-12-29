#ifndef DECORADOR_H
#define DECORADOR_H

#include <iostream>
#include "Task.h"
#include "Blackboard.h"

class Decorador: public Task{
public:
    Decorador();
    ~Decorador();
    virtual bool run(Blackboard* bb);
    bool terminate();
    void setChild(Task* newChild);
protected:
    Task* child;
};

class CountTime: public Decorador{
public:
	CountTime(int number);
	bool run(Blackboard* bb);
private:
	int contador;
	int maxContador;
};

#endif