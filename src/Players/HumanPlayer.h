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

	void GetNetInput();
	void SetNetInput();

	void Update();

	void ToggleMenu(bool newState);

private:
	void DeclareInput();
	
	PlayerController* controller;
	bool menuActivated;

};

#endif