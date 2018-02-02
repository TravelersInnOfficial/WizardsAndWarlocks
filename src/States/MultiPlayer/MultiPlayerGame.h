#ifndef MULTIPLAYERGAME_H
#define MULTIPLAYERGAME_H


#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"

#include "./../State.h"

class MultiPlayerGame: public State{
public:
	MultiPlayerGame();
	~MultiPlayerGame();

	bool Input();
	void Update(float deltaTime);
	void Draw();
private:
	State*	m_stateGame;

	SpellManager* 			spellManager;
	BulletManager* 			bulletManager;
	EffectManager* 			effectManager;
	ObjectManager*			objectManager;
	PlayerManager*			playerManager;
	TrapManager*			trapManager;
	NetworkManager*			networkManager;

	GraphicEngine*			g_engine;
};

#endif