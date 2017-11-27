#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "./Player.h"

#include "./ControlHechizo.h"
#include "./ControlProyectil.h"
#include "./ControlEffect.h"
#include "./ControlObject.h"
#include "./ControlPlayer.h"
//Fachada
#include "PhysicsEngine/BT_Body.h"
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
private:
	ControlHechizo* 	masterSpell;
	ControlProyectil* 	masterBullet;
	ControlEffect* 		masterEffect;
	ControlObject*		masterObject;
	ControlPlayer*		masterPlayer;

	Player*				playerOne;

	Game();
	static Game* instance;
};

#endif