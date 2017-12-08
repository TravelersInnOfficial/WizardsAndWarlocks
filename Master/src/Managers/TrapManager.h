#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include <iostream>
#include <vector>
#include "./../Trap.h"

class TrapManager{
public:
	static TrapManager* GetInstance();
	~TrapManager();

	void Update(float deltaTime);

	void DeleteTrap(Trap* t);
	Trap* AddTrap(vector3df pos,vector3df normal, TrapEnum type);
	void DeployTrap(TrapEnum type);
	void UpdateTrap(float deltaTime);

private:
	vector<Trap*> traps;

	TrapManager();
	static TrapManager* instance;
};

#endif