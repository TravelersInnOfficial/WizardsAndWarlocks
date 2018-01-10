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
#include "./../GraphicEngine/MenuManager.h"
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

	void RestartMatch();
	void CheckIfReady();

	float GetTotalTime();
	float GetDeltaTime();
	void UpdateDelta();

	//Sound Functions
	void playEvent(SoundEvent* event, vector3df pos);
	void playEvent(SoundEvent* event);

private:

	float timeStart;
	float deltaTime;
	float secondCounter;
	bool lobbyState;
	bool gameEnded;

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

	HumanPlayer*		playerOne;

	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
	void createSoundEvents();			//Create the sound events

	// PARA EL DEBUG
	AIPlayer* AL;

	void setFps();
	void CheckIfWon();
	
};

#endif