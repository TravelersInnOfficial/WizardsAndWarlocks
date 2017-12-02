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
	void ErasePlayer(int networkId);

private:

	vector<Player*>	players;

	ControlPlayer();
	static ControlPlayer* instance;
};

#endif