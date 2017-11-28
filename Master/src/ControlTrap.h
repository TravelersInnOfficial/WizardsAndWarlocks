#ifndef CONTROLTRAP_H
#define CONTROLTRAP_H

#include <iostream>
#include <vector>

#include "Trap.h"

class ControlTrap{
public:
	static ControlTrap* GetInstance();
	~ControlTrap();

	Trap* AddTrap(vector3df pos, vector3df dim, TrapEnum type);
private:
	vector<Trap*> traps;

	ControlTrap();
	static ControlTrap* instance;
};

#endif