#ifndef MANAGERTRAP_H
#define MANAGERTRAP_H

#include <iostream>
#include <vector>

#include "Trap.h"

class ManagerTrap{
public:
	static ManagerTrap* GetInstance();
	~ManagerTrap();

	void DeleteTrap(Trap* t);
	Trap* AddTrap(vector3df pos,TrapEnum type);
private:
	vector<Trap*> traps;

	ManagerTrap();
	static ManagerTrap* instance;
};

#endif