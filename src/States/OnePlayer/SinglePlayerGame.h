#ifndef SINGLEPLAYERGAME_H
#define SINGLEPLAYERGAME_H

#include "./../State.h"
#include <iostream>
#include <map>

class RegionalSenseManager;
class GraphicEngine;
class BulletManager;
class EffectManager;
class ObjectManager;
class PlayerManager;
class SpellManager;
class BulletEngine;
class TrapManager;
class SoundSystem;
class SoundEvent;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

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

	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
	void CreateSoundEvents();							//Create the sound events

};

#endif