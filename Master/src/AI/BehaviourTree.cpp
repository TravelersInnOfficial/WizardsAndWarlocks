#include "BehaviourTree.h"

BehaviourTree::BehaviourTree(){
    informacion = new Blackboard();

    Task* t = new Seek();
    SetRoot(t);

}

BehaviourTree::~BehaviourTree(){
	delete informacion;
	delete root;
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
