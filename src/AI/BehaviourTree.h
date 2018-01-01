#ifndef BEHAVIOURTREE_H
#define BEHAVIOURTREE_H

#include <iostream>
#include <string>
// Enums
#include <AICodes.h>
#include <KeyStates.h>
// BlackBoard
#include "Blackboard.h"
// Elementos del BehaviourTree
#include "Task.h"
#include "Selector.h"
#include "Secuencia.h"
#include "Decorador.h"
// Todas las tareas existentes
#include "./Tasks/AllTasks.h"


class BehaviourTree{
public:
    Blackboard* informacion;
    Task* rootAttack;
    Task* rootMove;

    BehaviourTree();
    ~BehaviourTree();
    void run();
    void SetRootAttack(Task* t);
    void SetRootMove(Task* t);
    void SetInformation(Blackboard* bb);
    void ResetInformacion();
    Blackboard* GetBlackboard();
    void AnyadirInformacion(AI_code name, void* value);
    void AnyadirInformacion(AI_code name, int value);
private:
	void CreateAttack();
	void CreateMovement();
};

#endif