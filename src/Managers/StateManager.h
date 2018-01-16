#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "./../States/Game.h"
#include "./../States/MenuPrincipal.h"
#include "./../States/NetGame.h"
#include "./../States/State.h"

#include <iostream>
#include <StateCodes.h>

class StateManager{
public:
	static StateManager* GetInstance();

	~StateManager();

	void PrepareStatus(State_Code status);
	bool Update();
	void LoadState(State_Code code);
private:
	State_Code 	preparedStatus;
	State*		currentState; 


	StateManager();
	static StateManager* instance;
};

#endif