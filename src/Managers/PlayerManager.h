#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <GraphicEngine/MenuManager.h>
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
	void RestartMatchStatus();
	void ResetAllSpells();
	void ManageMatchStatus(bool ManageMatchStatus);

	void AddToDead(Player* player);
	void AddToLife(Player* player);

	bool CheckIfWon(Alliance alliance);
	void CheckWon();
	void EraseAllCharacters();
	void ReturnAllToLobby();
	void RefreshServerAll();

	Player* GetPlayerOne();
	Player* GetPlayerFromID(int id);
	Player* GetPlayerFromNetID(int id);
	vector<Player*> GetAllPlayers();
	
	// DeadCam Functions
	bool PlayerAlive(Player* player);				// Da la posicion del jugador target, en el caso de que no este cambia el puntero
	Player* ChangePlayerTargetCam(Player* player);	// Cambia el jugador al que mirar en la camara de muerte

private:

	vector<Player*>	players;
	vector<Player*>	deadPlayers;

	bool wizardsWin;
	bool warlocksWin;

	vector<Player*> playersToDelete;

	PlayerManager();
	static PlayerManager* instance;
};

#endif