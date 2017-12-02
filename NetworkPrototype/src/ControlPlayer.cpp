#include "ControlPlayer.h"

ControlPlayer* ControlPlayer::instance = 0;

ControlPlayer* ControlPlayer::GetInstance(){
	if(instance==0){
		instance = new ControlPlayer();
	}
	return instance;
}

ControlPlayer::ControlPlayer(){}

ControlPlayer::~ControlPlayer(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		delete p;
	}
	players.clear();
}

Player* ControlPlayer::AddPlayer(bool one){
	Player* p = new Player(one);
	players.push_back(p);
	//std::cout<<"Creo el objeto visible"<<std::endl;
	return p;
}

void ControlPlayer::UpdatePlayers(){
	DeletePlayers();
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->Update();
	}
}

void ControlPlayer::AddToDeletePlayer(int networkId){
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

void ControlPlayer::DeletePlayers(){
	int size = playersToDelete.size();
	for(int i=0; i<size; i++){
		Player* p = playersToDelete[i];
		delete p;
	}
	playersToDelete.clear();
}