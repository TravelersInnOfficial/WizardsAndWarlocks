#ifndef NETGAME_H
#define NETGAME_H

#include <iostream>

#include "./../HumanPlayer.h"

#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/NetworkManager.h"

#include "./../PhysicsEngine/BulletEngine.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../SoundEngine/SoundSystem.h"
#include "./../NetworkEngine/NetworkEngine.h"

#include <SpellCodes.h>
#include <EffectCodes.h>

class NetGame{
public:

	static NetGame* GetInstance();
	~NetGame();
	bool Input();
	void Update();
	void Draw();

	float GetTotalTime();
	float GetDeltaTime();
	void UpdateDelta();

	void SetPlayerOne(NetworkObject* nObject);

private:

	static NetGame* instance;
	
	float timeStart;
	float deltaTime;
	float secondCounter = 0;

	bool isServer;
	SoundEvent*			footstepEvent;
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

};

#endif