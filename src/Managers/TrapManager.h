#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include <iostream>
#include <vector>
#include <vector3d.h>
#include "./../Objects/Trap.h"
#include "./../Players/Player.h"

class TrapManager{
public:
	static TrapManager* GetInstance();
	~TrapManager();

	void Update(float deltaTime);
	void DeleteTrap(Trap* t);
	
	void AddTrapToPlayer(Player*,TrapEnum);
	bool PlayerDeployTrap(Player*, vector3df, vector3df);

	Trap* AddTrap(vector3df pos,vector3df normal, TrapEnum type);
	bool DeployTrap(TrapEnum type, vector3df, vector3df);
	void UpdateTrap(float deltaTime);

	int getPlayerUsings(Player*);
	TrapEnum getPlayerTrap(Player*);

	bool setPlayerUsings(Player*, int);
	bool setPlayerTrap(Player*, TrapEnum);

	void ClearTraps();

private:
	vector<Trap*> traps;
	std::map<Player*,TrapEnum> playerTrap;
	std::map<Player*, int> playerUsings;

	TrapManager();
	static TrapManager* instance;

	int MaxUsings = 4;
};

#endif