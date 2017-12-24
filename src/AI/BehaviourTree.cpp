#include "BehaviourTree.h"

BehaviourTree::BehaviourTree(){
    informacion = new Blackboard();

    Task* t1 	= new T_Seek();
    Task* t2 	= new CheckPlayers();
    Task* t3    = new HasArrived();

    Secuencia* s1	= new Secuencia();
    s1->addChild(t2);
    s1->addChild(t1);
    s1->addChild(t3);

    Task* t4        = new T_Wander();
    Selector* s2   = new Selector();

    s2->addChild(s1);
    s2->addChild(t4);

    SetRoot(s2);

}

BehaviourTree::~BehaviourTree(){
	delete informacion;
}

void BehaviourTree::run(){
    root->run(informacion);
}

void BehaviourTree::SetRoot(Task* t){
    root = t;
}

void BehaviourTree::AnyadirInformacion(AI_code name, void* value){
    informacion->SetPuntero(name, value);
}

void BehaviourTree::AnyadirInformacion(AI_code name, int value){
    informacion->SetInt(name, value);
}

Blackboard* BehaviourTree::GetBlackboard(){
	return informacion;
}
