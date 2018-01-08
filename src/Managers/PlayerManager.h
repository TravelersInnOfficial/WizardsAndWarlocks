#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "./../GraphicEngine/MenuManager.h"
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
	void RespawnDeadPlayers();
	void RespawnAll();
	void ManageMatchStatus(bool ManageMatchStatus);
	void AddToDead(Alliance alliance, Player* player);
	bool CheckIfWon(Alliance alliance);
	void CheckWon();
	void EraseAllCharacters();
	void ReturnAllToLobby();
	void RefreshServerAll();
	void UpdateNetDebug();
	Player* GetPlayerFromID(int id);
	
private:

	vector<Player*>	players;
	vector<Player*>	wizardPlayers;
	vector<Player*>	warlockPlayers;

	vector<Player*>	deadPlayers;
	vector<Player*>	deadWarlocks;
	vector<Player*>	deadWizards;

	bool wizardsWin;
	bool warlocksWin;

	vector<Player*> playersToDelete;

	PlayerManager();
	static PlayerManager* instance;
};

#endif