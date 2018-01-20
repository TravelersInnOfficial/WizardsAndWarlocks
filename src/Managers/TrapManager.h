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
	bool DeployTrap(TrapEnum type,vector3df Start, vector3df End, int playerId);
	void UpdateTrap(float deltaTime);

	int getPlayerUsings(Player*);
	TrapEnum getPlayerTrap(Player*);
	std::string GetPathFromEnum(TrapEnum tKind);
	vector<Trap*> GetAllTraps();

	bool setPlayerUsings(Player*, int);
	bool setPlayerTrap(Player*, TrapEnum, bool broadcast = true);

	void ClearTraps();
	void SendAllSignal();

	// FOR NETWORK
	void DirectDeploy(int playerId, vector3df position, vector3df normal, int id);
	void NoPlayerDeploy(vector3df position, vector3df normal, TrapEnum type, int id);

	void IdErase(int id);
	Trap* GetTrapWithId(int id);

	void DrawHUD(Player* player);

	void ErasePlayer(Player* player);
	void RefreshServerAll();

private:
	vector<Trap*> traps;
	std::map<Player*,TrapEnum> playerTrap;
	std::map<Player*, int> playerUsings;
	int lastTrapId;
	static TrapManager* instance;
	int MaxUsings = 4;

	TrapManager();
	std::string GetPathFromUsings(int usings);
};

#endif