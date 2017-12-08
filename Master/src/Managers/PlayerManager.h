#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <iostream>
#include <vector>

#include "./../Player.h"
#include "./../HumanPlayer.h"
#include "./../AIPlayer.h"

class PlayerManager{
public:
	static PlayerManager* GetInstance();
	~PlayerManager();

	Player* AddHumanPlayer(bool isPlayer1 = true);
	Player* AddAIPlayer();
	void UpdatePlayers(bool isNetGame = false);
	void AddToDeletePlayer(int networkId);
	void DeletePlayers();

	Player* GetOne(){return players[0];}

private:

	vector<Player*>	players;
	vector<Player*> playersToDelete;

	PlayerManager();
	static PlayerManager* instance;
};

#endif