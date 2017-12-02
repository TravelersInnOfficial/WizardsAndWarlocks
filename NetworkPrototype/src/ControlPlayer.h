#ifndef CONTROLPLAYER_H
#define CONTROLPLAYER_H

#include <iostream>
#include <vector>

#include "Player.h"

class ControlPlayer{
public:
	static ControlPlayer* GetInstance();
	~ControlPlayer();

	Player* AddPlayer(bool);
	void UpdatePlayers();
	void AddToDeletePlayer(int networkId);
	void DeletePlayers();

private:

	vector<Player*>	players;
	vector<Player*> playersToDelete;

	ControlPlayer();
	static ControlPlayer* instance;
};

#endif