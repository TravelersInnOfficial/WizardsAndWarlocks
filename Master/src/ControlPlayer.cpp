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
	return p;
}

void ControlPlayer::UpdatePlayers(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->Update();
	}
}