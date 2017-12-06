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
private:

	vector<Player*>	players;

	PlayerManager();
	static PlayerManager* instance;
};

#endif