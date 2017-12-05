#include "ManagerPlayer.h"

ManagerPlayer* ManagerPlayer::instance = 0;

ManagerPlayer* ManagerPlayer::GetInstance(){
	if(instance==0){
		instance = new ManagerPlayer();
	}
	return instance;
}

ManagerPlayer::ManagerPlayer(){}

ManagerPlayer::~ManagerPlayer(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		delete p;
	}
	players.clear();
}

Player* ManagerPlayer::AddPlayer(bool one){
	Player* p = new Player(one);
	players.push_back(p);
	return p;
}

void ManagerPlayer::UpdatePlayers(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->Update();
	}
}