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

    void Update();
    void Run();

    // Setters
    void SetRootReceive(Task* t);
    void SetRootAction(Task* t);
    void SetRootMove(Task* t);
    void SetInformation(Blackboard* bb);
    void SetPlayer(AIPlayer* p);

    // Functions
    void ResetInformacion();
    void LoadRoomGraph();

    // Getters
    Blackboard* GetBlackboard();
    RoomGraph* GetRoomGraph();

    // Adders
    void AnyadirInformacion(AI_code name, void* value);
    void AnyadirInformacion(AI_code name, int value);
   

private:
    std::vector<Task*> tasks;        // Vector con todas las tareas del behaviour

    void CreateReceive();
	void CreateAction();
	void CreateMovement();
    void PrepareSubTrees();

    void CreateNoMove();        // SubArbol de no movimiento
    void CreateMoveSpell();     // SubArboles de movimiento con hechizos
    void CreateMoveDefault();   // SubArbol de movimiento por defecto
    void CreateMoveToTarget();  // SubArbol de movimiento hacia el target
    void CreatePathToTarget();  // SubArbol de movimiento hacia el target con path
    void CreateShootSpell();    // SubArbol de lanzar hechizo
    void CreateCathPotion();    // SubArbol de coger una pocion
    void CreateDrinkPotion();   // SubArbol de beber una pocion
    void CreateUseFountain();   // SubArbol de usar una fuente
    void CreateDefuseTrap();    // SubArbol de desactivar trampas
    void CreateEscapeMove();    // SubArbol de huir
    void CreateExploreTask();   // SubArbol de tarea explorar
    void CreateExploreMove();   // SubArbol de exploracion
    void CreateTravelTask();    // SubArbol de mirar a que habitacion ir
    void CreateTravelMove();    // SubArbol de moverse hasta la habitacion
    void CreateOpenDoor();      // SubArbol de movimiento para ponerse delante de la puerta
    void CreateMoveFountain();  // SubArbol de movimiento para moverse delante de una fuente
    void CreateCatchGrail();    // SubArbol de coger un grial
    void CreateMoveInteract();  // SubArbol de movimiento para interactuar
};

#endif