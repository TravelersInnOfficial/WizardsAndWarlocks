#ifndef MULTIMATCH_H
#define MULTIMATCH_H

#include "./MultiPlayerGame.h"
#include <Alliance.h>

class NetworkObject;
class Player;

class MultiMatch: public State{
public:
	MultiMatch(MultiPlayerGame* fat);
	~MultiMatch();

	bool Input();
	void Update(float deltaTime);
	void Draw();
private:
	void CheckIfWon();
	void MatchEnded(Alliance winnerAlliance);

	// Nos guardamos los managers que hay actualmente creados
	// Pero sera el SinglePlayerGame quien se encargara de eliminarlos
	
	SpellManager* 			spellManager;
	BulletManager* 			bulletManager;
	EffectManager* 			effectManager;
	ObjectManager*			objectManager;
	PlayerManager*			playerManager;
	TrapManager*			trapManager;
	NetworkManager*		 	networkManager;

	GraphicEngine*		g_engine;
	BulletEngine*		f_engine;
	SoundSystem*		s_engine;
	NetworkEngine*		n_engine;

	MultiPlayerGame* 	father;

	Player* playerOne;
	NetworkObject* 	networkObject;
	bool gameEnded;
	bool winnerMenuCreated;

	bool	isServer;
};

#endif