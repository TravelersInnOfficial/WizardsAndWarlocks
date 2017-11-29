#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "./Player.h"

#include "./ControlHechizo.h"
#include "./ControlProyectil.h"
#include "./ControlEffect.h"
#include "./ControlObject.h"
#include "./ControlPlayer.h"
#include "./ControlTrap.h"
//Fachada
#include "PhysicsEngine/BulletEngine.h"
#include "GraphicEngine/GraphicEngine.h"
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

	float GetDeltaTime();
	float GetTotalTime();
	void UpdateDelta();
private:

	float timeStart;
	float deltaTime;

	ControlHechizo* 	masterSpell;
	ControlProyectil* 	masterBullet;
	ControlEffect* 		masterEffect;
	ControlObject*		masterObject;
	ControlPlayer*		masterPlayer;
	ControlTrap*		masterTrap;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;

	Player*				playerOne;

	Game();
	static Game* instance;
};

#endif