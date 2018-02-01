#ifndef NETGAME_H
#define NETGAME_H

#include <iostream>

#include "./../Players/HumanPlayer.h"

#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/NetworkManager.h"

#include <PhysicsEngine/BulletEngine.h>
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <NetworkEngine/NetworkEngine.h>

#include "./../LevelLoader.h"
#include "./State.h"

#include <SpellCodes.h>
#include <EffectCodes.h>

class NetGame: public State{
public:

	static NetGame* GetInstance();
	~NetGame();
	bool Input();
	void Update(float deltaTime);
	void Draw();

	void RestartMatch();
	void CheckIfReady();
	void MatchEnded(Alliance winnerAlliance);

	float GetTotalTime();
	float GetDeltaTime();
	void UpdateDelta();

	void CreatePlayer(NetworkObject* nObject, bool isPlayerOne = false, std::string name = "");
	bool GetLobbyState();

	//Sound Functions
	void playEvent(SoundEvent* event, vector3df pos);
	void playEvent(SoundEvent* event);

private:

	static NetGame* instance;
	
	float timeStart;
	float deltaTime;
	float secondCounter;
	bool lobbyState;
	bool gameEnded;
	bool debug;
	bool mute;
	bool captured;

	bool isServer;
	HumanPlayer*		playerOne;

	SpellManager* 		spellManager;
	BulletManager* 		bulletManager;
	EffectManager* 		effectManager;
	ObjectManager*		objectManager;
	PlayerManager*		playerManager;
	TrapManager*		trapManager;
	NetworkManager*		networkManager;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;
	NetworkEngine*		n_engine;

	NetGame();

	void setFps();
	void CheckIfWon();

	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
	void createSoundEvents();			//Create the sound events

};

#endif