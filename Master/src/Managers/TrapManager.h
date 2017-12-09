#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include <iostream>
#include <vector>
#include "./../Trap.h"
#include "./../Player.h"

class TrapManager{
public:
	static TrapManager* GetInstance();
	~TrapManager();

	void Update(float deltaTime);
	void DeleteTrap(Trap* t);
	
	void AddTrapToPlayer(Player*,TrapEnum);
	bool PlayerDeployTrap(Player*);

	Trap* AddTrap(vector3df pos,vector3df normal, TrapEnum type);
	void DeployTrap(TrapEnum type);
	void UpdateTrap(float deltaTime);

	int getPlayerUsings(Player*);
	TrapEnum getPlayerTrap(Player*);

	bool setPlayerUsings(Player*, int);
	bool setPlayerTrap(Player*, TrapEnum);

private:
	vector<Trap*> traps;
	std::map<Player*,TrapEnum> playerTrap;
	std::map<Player*, int> playerUsings;

	TrapManager();
	static TrapManager* instance;

	int MaxUsings = 4;
};

#endif