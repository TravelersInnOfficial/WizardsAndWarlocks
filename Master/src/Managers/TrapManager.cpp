#include "TrapManager.h"

TrapManager* TrapManager::instance = 0;

TrapManager* TrapManager::GetInstance(){
	if(instance==0){
		instance = new TrapManager();
	}
	return instance;
}

TrapManager::TrapManager(){}

TrapManager::~TrapManager(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();
}

Trap* TrapManager::AddTrap(vector3df pos, TrapEnum type){
	Trap* t = new Trap(pos, type);
	traps.push_back(t);
	return t;
}

void TrapManager::DeleteTrap(Trap* trap){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		if(t == trap){
			traps.erase(traps.begin() + i);
			delete t;
		}
	}
}