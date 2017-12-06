#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <iostream>
#include <vector>

#include "./../Player.h"

class PlayerManager{
public:
	static PlayerManager* GetInstance();
	~PlayerManager();

	Player* AddPlayer(bool);
	void UpdatePlayers();
	void AddToDeletePlayer(int networkId);
	void DeletePlayers();

private:

	vector<Player*>	players;
	vector<Player*> playersToDelete;

	PlayerManager();
	static PlayerManager* instance;
};

#endif