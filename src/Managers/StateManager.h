#ifndef STATEMANAGER_H
#define STATEMANAGER_H

// Estados
#include "./../States/Game.h"
#include "./../States/MenuPrincipal.h"
#include "./../States/NetGame.h"
#include "./../States/State.h"
// Motores
#include "./../PhysicsEngine/BulletEngine.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../SoundEngine/SoundSystem.h"

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

	BulletEngine*	f_engine;
	GraphicEngine*	g_engine;
	SoundSystem*	s_engine;
	NetworkEngine*	n_engine;

	StateManager();
	static StateManager* instance;
};

#endif