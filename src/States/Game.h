#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "./../LevelLoader.h"
#include "./../Players/HumanPlayer.h"
#include "./../Npcs/Npc.h"
#include "./../Npcs/NpcSelector.h"

#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

#include "./../PhysicsEngine/BulletEngine.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../SoundEngine/SoundSystem.h"

//Enums
#include <SpellCodes.h>
#include <EffectCodes.h>
#include <NPCTypes.h>

#include "./../Players/AIPlayer.h"

class Game{
public:

	Game();
	~Game();
	bool Input();
	void Update();
	void Draw();

	float GetTotalTime();
	float GetDeltaTime();
	void UpdateDelta();

private:

	float timeStart;
	float deltaTime;
	float secondCounter = 0;

	SpellManager* 			spellManager;
	BulletManager* 			bulletManager;
	EffectManager* 			effectManager;
	ObjectManager*			objectManager;
	PlayerManager*			playerManager;
	TrapManager*			trapManager;
	RegionalSenseManager* 	senseManager;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;
	SoundEvent*			footstepEvent;

	HumanPlayer*				playerOne;

	// PARA EL DEBUG
	AIPlayer* AL;

	void setFps();
	
};

#endif