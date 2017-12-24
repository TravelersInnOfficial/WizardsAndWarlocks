#ifndef BEHAVIOURTREE_H
#define BEHAVIOURTREE_H

#include <iostream>
#include <string>
#include <AICodes.h>
#include "Blackboard.h"
#include "Task.h"
#include "./Tasks/AllTasks.h"
#include "Secuencia.h"
#include "Selector.h"
#include "Decorador.h"

class BehaviourTree{
public:
    Blackboard* informacion;
    Task* root;

    BehaviourTree();
    ~BehaviourTree();
    void run();
    void SetRoot(Task* t);
    Blackboard* GetBlackboard();
    void AnyadirInformacion(AI_code name, void* value);
    void AnyadirInformacion(AI_code name, int value);
private:

};

#endif