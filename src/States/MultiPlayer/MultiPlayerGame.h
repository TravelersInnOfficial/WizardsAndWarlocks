#ifndef MULTIPLAYERGAME_H
#define MULTIPLAYERGAME_H


#include "./../Managers/NetworkManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/TrapManager.h"

#include "./../State.h"
#include "./../LevelLoader.h"

class MultiPlayerGame: public State{
public:
	MultiPlayerGame();
	~MultiPlayerGame();

	void StartGame();
	void ReturnLobby();

	bool Input();
	void Update(float deltaTime);
	void Draw();
private:
	void CleanGame();
	void ChangeMode();

	State*	m_stateGame;

	SpellManager* 			spellManager;
	BulletManager* 			bulletManager;
	EffectManager* 			effectManager;
	ObjectManager*			objectManager;
	PlayerManager*			playerManager;
	TrapManager*			trapManager;
	NetworkManager*			networkManager;

	GraphicEngine*			g_engine;
	BulletEngine*			f_engine;
	NetworkEngine*			n_engine;

	bool 	captured;
	bool 	mute;
	bool 	debug;

	int 	m_changeMode;
};

#endif