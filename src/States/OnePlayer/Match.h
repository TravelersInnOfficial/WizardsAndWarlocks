#ifndef MATCH_H
#define MATCH_H

#include "./SinglePlayerGame.h"

class Player;

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
	RegionalSenseManager* 	senseManager;
	ObjectManager*			objectManager;
	BulletManager* 			bulletManager;
	PlayerManager*			playerManager;
	EffectManager* 			effectManager;
	SpellManager* 			spellManager;
	TrapManager*			trapManager;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;

	SinglePlayerGame* 	father;

	Player* playerOne;
	bool gameEnded;
};

#endif