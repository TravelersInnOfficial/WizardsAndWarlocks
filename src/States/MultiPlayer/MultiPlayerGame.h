#ifndef MULTIPLAYERGAME_H
#define MULTIPLAYERGAME_H

#include "./../State.h"
#include <iostream>
#include <map>

class NetworkManager;
class BulletManager;
class EffectManager;
class ObjectManager;
class PlayerManager;
class GraphicEngine;
class NetworkEngine;
class SpellManager;
class BulletEngine;
class SoundSystem;
class TrapManager;
class SoundEvent;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class MultiPlayerGame: public State{
public:
	MultiPlayerGame();
	~MultiPlayerGame();

	void StartGame();
	void ReturnLobby();

	bool Input();
	void Update(float deltaTime);
	void Draw();

	//Sound Functions
	void PlayEvent(std::string event, vector3df pos);
	void PlayEvent(std::string event);
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
	SoundSystem*			s_engine;

	bool 	captured;
	bool 	mute;
	bool 	debug;

	int 	m_changeMode;

	bool m_isServer;

	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
	void CreateSoundEvents();							//Create the sound events
};

#endif