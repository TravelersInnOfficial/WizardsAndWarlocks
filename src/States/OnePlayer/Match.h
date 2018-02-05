#ifndef MATCH_H
#define MATCH_H

#include "./../State.h"
#include "./SinglePlayerGame.h"

class Match: public State{
public:
	Match(SinglePlayerGame* fat);
	~Match();

	bool Input();
	void Update(float deltaTime);
	void Draw();

private:
	void CheckIfWon();

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
	bool gameEnded;
};

#endif