#include "ManagerTrap.h"

ManagerTrap* ManagerTrap::instance = 0;

ManagerTrap* ManagerTrap::GetInstance(){
	if(instance==0){
		instance = new ManagerTrap();
	}
	return instance;
}

ManagerTrap::ManagerTrap(){}

ManagerTrap::~ManagerTrap(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();
}

Trap* ManagerTrap::AddTrap(vector3df pos, vector3df dim, TrapEnum type){
	Trap* t = new Trap(pos, dim, type);
	traps.push_back(t);
	return t;
}

void ManagerTrap::DeleteTrap(Trap* trap){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		if(t == trap){
			traps.erase(traps.begin() + i);
			delete t;
		}
	}
}