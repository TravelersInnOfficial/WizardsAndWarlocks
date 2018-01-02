#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "./PlayerController.h"
#include "Player.h"
#include <KeyStates.h>

struct ActionHolder{
	ACTION_ENUM up_key;
	ACTION_ENUM down_key;
	ACTION_ENUM right_key;
	ACTION_ENUM left_key;

	ActionHolder(){
		up_key = ACTION_MOVE_UP;
		down_key = ACTION_MOVE_DOWN;
		right_key = ACTION_MOVE_RIGHT;
		left_key = ACTION_MOVE_LEFT;
	}
};

class HumanPlayer: public Player{
public:
	HumanPlayer(bool isPlayer1 = true);
	~HumanPlayer();

	void CheckInput();

	void GetNetInput();
	void SetNetInput();

	void Update();

	void ToggleMenu(bool newState);

	ActionHolder playerActions;

private:
	bool menuActivated;

};

#endif