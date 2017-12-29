#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <iostream>
#include <vector>

#include "./../Players/Player.h"
#include "./../Players/HumanPlayer.h"
#include "./../Players/AIPlayer.h"

class PlayerManager{
public:
	static PlayerManager* GetInstance();
	~PlayerManager();

	Player* AddHumanPlayer(bool isPlayer1 = true);
	AIPlayer* AddAIPlayer();
	void UpdatePlayers(bool isNetGame = false);
	void AddToDeletePlayer(int networkId);
	void DeletePlayers();
	void SendVisualSignal();
	bool CheckIfReady();
	void RespawnAll();
	
private:

	vector<Player*>	players;
	vector<Player*>	wizardPlayers;
	vector<Player*>	warlockPlayers;
	vector<Player*> playersToDelete;

	PlayerManager();
	static PlayerManager* instance;
};

#endif