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
	warlockPlayers.push_back(p);
	p->SetAlliance(ALLIANCE_WARLOCK);
	return p;
}

AIPlayer* PlayerManager::AddAIPlayer(){
	AIPlayer* p = new AIPlayer();
	players.push_back(p);
	wizardPlayers.push_back(p);
	p->SetAlliance(ALLIANCE_WIZARD);
	return p;
}

void PlayerManager::UpdatePlayers(bool isNetGame){
	DeletePlayers();
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(isNetGame) {
			HumanPlayer* hp = (HumanPlayer*) players[i];
			if(p->IsPlayerOne()) hp->SetNetInput();
			else hp->GetNetInput();
		}
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

void PlayerManager::SendVisualSignal(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->SendSignal();
	}
}