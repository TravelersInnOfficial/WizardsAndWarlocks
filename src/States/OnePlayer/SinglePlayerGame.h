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
#include "./../LevelLoader.h"

#include <SoundEngine/SoundSystem.h>

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

	//Sound Functions
	void PlayEvent(std::string event, vector3df pos);
	void PlayEvent(std::string event);
private:
	void ChangeMode();

	State*	m_stateGame;

	SpellManager* 			spellManager;
	BulletManager* 			bulletManager;
	EffectManager* 			effectManager;
	ObjectManager*			objectManager;
	PlayerManager*			playerManager;
	TrapManager*			trapManager;
	RegionalSenseManager* 	senseManager;

	GraphicEngine*			g_engine;
	BulletEngine*			f_engine;
	SoundSystem*			s_engine;

	bool 	captured;
	bool 	mute;
	bool 	debug;

	int 	m_changeMode;

	AIPlayer* AL;

	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
	void CreateSoundEvents();							//Create the sound events

};

#endif