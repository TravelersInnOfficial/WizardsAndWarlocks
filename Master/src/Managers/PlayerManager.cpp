#include "PlayerManager.h"

PlayerManager* PlayerManager::instance = 0;

PlayerManager* PlayerManager::GetInstance(){
	if(instance==0) instance = new PlayerManager();
	return instance;
}

PlayerManager::PlayerManager(){}

PlayerManager::~PlayerManager(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		delete p;
	}
	players.clear();
}

Player* PlayerManager::AddHumanPlayer(bool isPlayer1){
	Player* p = new HumanPlayer(isPlayer1);
	players.push_back(p);
	return p;
}

Player* PlayerManager::AddAIPlayer(){
	Player* p = new AIPlayer();
	players.push_back(p);
	return p;
}

void PlayerManager::UpdatePlayers(){
	DeletePlayers();
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->Update();
	}
}

void PlayerManager::AddToDeletePlayer(int networkId){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(p->GetNetworkObject()->GetObjId() == networkId){
			players.erase(players.begin()+i);
			playersToDelete.push_back(p);
			break;
		}
	}
}

void PlayerManager::DeletePlayers(){
	int size = playersToDelete.size();
	for(int i=0; i<size; i++){
		Player* p = playersToDelete[i];
		delete p;
	}
	playersToDelete.clear();
}