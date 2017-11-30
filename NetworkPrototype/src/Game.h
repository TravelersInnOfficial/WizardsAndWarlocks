#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "./Player.h"

//Controllers
#include "./ControlHechizo.h"
#include "./ControlProyectil.h"
#include "./ControlEffect.h"
#include "./ControlObject.h"
#include "./ControlPlayer.h"
#include "./ControlTrap.h"
#include "./ControlNetwork.h"

//Fachada
#include "PhysicsEngine/BulletEngine.h"
#include "GraphicEngine/GraphicEngine.h"
#include "SoundEngine/SoundSystem.h"

//Enums
#include "./Spells/SpellCodes.h"
#include "./Effects/EffectCodes.h"

class Game{
public:
	static Game* GetInstance(bool isServer = false);
	~Game();

	bool Input();
	void Update();
	void Draw();

	float GetDeltaTime();
	void UpdateDelta();
private:

	float timeStart;
	float deltaTime;
	bool isServer;

	ControlNetwork*		masterNetwork;
	ControlHechizo* 	masterSpell;
	ControlProyectil* 	masterBullet;
	ControlEffect* 		masterEffect;
	ControlObject*		masterObject;
	ControlPlayer*		masterPlayer;
	ControlTrap*		masterTrap;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;
	SoundEvent*			footstepEvent;

	Player*				playerOne;

	Game(bool isServer);
	static Game* instance;
};

#endif