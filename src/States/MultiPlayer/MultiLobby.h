#ifndef MULTILOBBY_H
#define MULTILOBBY_H

#include "./MultiPlayerGame.h"

class NetworkObject;
class Player;

class MultiLobby: public State{
public:
	MultiLobby(MultiPlayerGame* fat);
	~MultiLobby();

	bool Input();
	void Update(float deltaTime);
	void Draw();
private:
	void UpdateLobby(float deltaTime);
	void CheckIfReady();
	
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

	Player* 		playerOne;
	NetworkObject* 	networkObject;

	bool	isServer;
};


#endif