#ifndef SINGLEPLAYERGAME_H
#define SINGLEPLAYERGAME_H

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"

#include "./../State.h"

class SinglePlayerGame: public State{
public:
	SinglePlayerGame();
	~SinglePlayerGame();

	void StartGame();
	void ReturnLobby();
	void CleanGame();

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
	RegionalSenseManager* 	senseManager;

	GraphicEngine*			g_engine;
};

#endif