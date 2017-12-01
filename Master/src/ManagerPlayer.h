#ifndef MANAGERPLAYER_H
#define MANAGERPLAYER_H

#include <iostream>
#include <vector>

#include "Player.h"

class ManagerPlayer{
public:
	static ManagerPlayer* GetInstance();
	~ManagerPlayer();

	Player* AddPlayer(bool);
	void UpdatePlayers();
private:

	vector<Player*>	players;

	ManagerPlayer();
	static ManagerPlayer* instance;
};

#endif