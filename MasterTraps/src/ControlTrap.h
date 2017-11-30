#ifndef CONTROLTRAP_H
#define CONTROLTRAP_H

#include <iostream>
#include <vector>

#include "Trap.h"

class ControlTrap{
public:
	static ControlTrap* GetInstance();
	~ControlTrap();

	void DeleteTrap(Trap* t);
	Trap* AddTrap(vector3df pos, TrapEnum type);
private:
	vector<Trap*> traps;

	ControlTrap();
	static ControlTrap* instance;
};

#endif