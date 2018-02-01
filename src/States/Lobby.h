#ifndef LOBBY_H
#define LOBBY_H

// Motores
#include <PhysicsEngine/BulletEngine.h>
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/MenuManager.h>
#include <SoundEngine/SoundSystem.h>
// Managers
#include "./../Managers/SpellManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

#include "./State.h"
#include "./SinglePlayerGame.h"

#include "./../LevelLoader.h"

class Lobby: public State{
public:
	Lobby(SinglePlayerGame* fat);
	~Lobby();

	bool Input();
	void Update(float deltaTime);
	void Draw();
private:
	void CheckIfReady();
	
	// Nos guardamos los managers que hay actualmente creados
	// Pero sera el SinglePlayerGame quien se encargara de eliminarlos
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

	SinglePlayerGame* 	father;

	Player* playerOne;
};

#endif