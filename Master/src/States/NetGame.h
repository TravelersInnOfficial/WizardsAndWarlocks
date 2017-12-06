#ifndef NETGAME_H
#define NETGAME_H

#include <iostream>

#include "./../Player.h"

#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"

//Fachada
#include "./../PhysicsEngine/BulletEngine.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../SoundEngine/SoundSystem.h"
#include "./../NetworkEngine/NetworkEngine.h"

//Enums
#include <SpellCodes.h>
#include <EffectCodes.h>

class NetGame{
public:

	NetGame();
	~NetGame();
	bool Input();
	void Update();
	void Draw();

	float GetTotalTime();
	float GetDeltaTime();
	void UpdateDelta();

private:

	float timeStart;
	float deltaTime;
	bool isServer;

	SpellManager* 		spellManager;
	BulletManager* 		bulletManager;
	EffectManager* 		effectManager;
	ObjectManager*		objectManager;
	PlayerManager*		playerManager;
	TrapManager*		trapManager;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;
	NetworkEngine*		n_engine;

	SoundEvent*			footstepEvent;

	Player*				playerOne;

	void SetPlayerOne(NetworkObject* nObject);

};

#endif