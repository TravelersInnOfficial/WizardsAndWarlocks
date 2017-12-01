#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "./Player.h"

#include "./ManagerSpell.h"
#include "./ManagerBullet.h"
#include "./ManagerEffect.h"
#include "./ManagerObject.h"
#include "./ManagerPlayer.h"
#include "./ManagerTrap.h"
//Fachada
#include "PhysicsEngine/BulletEngine.h"
#include "GraphicEngine/GraphicEngine.h"
#include "SoundEngine/SoundSystem.h"
//Enums
#include "./Spells/SpellCodes.h"
#include "./Effects/EffectCodes.h"

class Game{
public:
	static Game* GetInstance();

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

	ManagerSpell* 		masterSpell;
	ManagerBullet* 		masterBullet;
	ManagerEffect* 		masterEffect;
	ManagerObject*		masterObject;
	ManagerPlayer*		masterPlayer;
	ManagerTrap*		masterTrap;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;
	SoundEvent*			footstepEvent;

	Player*				playerOne;

	Game();
	static Game* instance;
};

#endif