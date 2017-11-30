#include "ControlTrap.h"

ControlTrap* ControlTrap::instance = 0;

ControlTrap* ControlTrap::GetInstance(){
	if(instance==0){
		instance = new ControlTrap();
	}
	return instance;
}

ControlTrap::ControlTrap(){}

ControlTrap::~ControlTrap(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();
}

Trap* ControlTrap::AddTrap(vector3df pos,TrapEnum type){
	Trap* t = new Trap(vector3df(pos.X,-0.49,pos.Z), type);
	traps.push_back(t);
	return t;
}

void ControlTrap::DeleteTrap(Trap* trap){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		if(t == trap){
			traps.erase(traps.begin() + i);
			delete t;
		}
	}
}