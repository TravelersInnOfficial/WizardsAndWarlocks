#ifndef LOBBY_H
#define LOBBY_H

#include "./../State.h"
#include "./SinglePlayerGame.h"

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