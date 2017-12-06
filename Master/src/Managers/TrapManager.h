#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include <iostream>
#include <vector>
#include "./../Trap.h"

class TrapManager{
public:
	static TrapManager* GetInstance();
	~TrapManager();

	void DeleteTrap(Trap* t);
	Trap* AddTrap(vector3df pos,TrapEnum type);
private:
	vector<Trap*> traps;

	TrapManager();
	static TrapManager* instance;
};

#endif