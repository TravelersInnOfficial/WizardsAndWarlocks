#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <Alliance.h>
#include <iostream>
#include <vector>

class Player;
class AIPlayer;

class PlayerManager{
public:
	static PlayerManager* GetInstance();
	~PlayerManager();
	void InitObject();
	void EmptyObject();

	Player* AddHumanPlayer(bool isPlayer1 = true);
	AIPlayer* AddAIPlayer();
	void UpdatePlayers(float deltaTime, bool isNetGame = false);
	void AddToDeletePlayer(int networkId);
	void DeletePlayers();
	void SendVisualSignal();
	bool CheckIfReady();
	void RespawnDeadPlayers();

	void InitGame();
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
	void DrawDebug();

	void SpawnAI();
	void EraseAI();

	Player* GetPlayerOne();
	Player* GetPlayerFromID(int id);
	Player* GetPlayerFromNetID(int id);
	std::vector<Player*> GetAllPlayers();
	std::vector<Player*> GetAllPlayers(Alliance alli);
	
	// DeadCam Functions
	bool PlayerAlive(Player* player);				// Da la posicion del jugador target, en el caso de que no este cambia el puntero
	Player* ChangePlayerTargetCam(Player* player, Alliance alli);	// Cambia el jugador al que mirar en la camara de muerte

private:
	Player*					playerOne;
	std::vector<Player*>	players;
	std::vector<Player*>	deadPlayers;
	std::vector<Player*>	playersToDelete;

	bool wizardsWin;
	bool warlocksWin;

	PlayerManager();
	PlayerManager(PlayerManager&);
	PlayerManager operator =(PlayerManager&);
};

#endif