#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H


#include "./ControllerPlayer.h"
#include "Player.h"

class HumanPlayer: public Player{
public:
	HumanPlayer();
	~HumanPlayer();

	void UpdateInput();
	void CheckInput();

	void Update();

private:
	void DeclareInput();
	
	ControllerPlayer* controller;

};

#endif