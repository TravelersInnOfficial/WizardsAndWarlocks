#ifndef BEHAVIOURTREE_H
#define BEHAVIOURTREE_H

#include <iostream>
#include <string>
#include <vector>
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
// Player
class AIPlayer;

class BehaviourTree{
public:
    Blackboard* informacion;
    Task* rootRecive;
    Task* rootAction;
    Task* rootMove;

    BehaviourTree();
    ~BehaviourTree();
    void run();

    // Setters
    void SetRootReceive(Task* t);
    void SetRootAction(Task* t);
    void SetRootMove(Task* t);
    void SetInformation(Blackboard* bb);
    void SetPlayer(AIPlayer* p);

    // Functions
    void ResetInformacion();

    // Getters
    Blackboard* GetBlackboard();

    // Adders
    void AnyadirInformacion(AI_code name, void* value);
    void AnyadirInformacion(AI_code name, int value);
   

private:
    std::vector<Task*> tasks;        // Vector con todas las tareas del behaviour

    void CreateReceive();
	void CreateAction();
	void CreateMovement();
    void PrepareSubTrees();

    void CreateMoveSpell();     // SubArboles de movimiento con hechizos
    void CreateMoveDefault();   // SubArbol de movimiento por defecto
    void CreateMoveToTarget();  // SubArbol de movimiento hacia el target
    void CreateShootSpell();    // SubArbol de lanzar hechizo
    void CreateCathPotion();    // SubArbol de coger una pocion
    void CreateDrinkPotion();   // SubArbol de beber una pocion
};

#endif