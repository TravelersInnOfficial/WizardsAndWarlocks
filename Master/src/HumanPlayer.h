#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H


#include "./PlayerController.h"
#include "Player.h"

class HumanPlayer: public Player{
public:
	HumanPlayer(bool isPlayer1 = true);
	~HumanPlayer();

	void UpdateInput();
	void CheckInput();

	void Update();

private:
	void DeclareInput();
	
	PlayerController* controller;

};

#endif