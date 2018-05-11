#ifndef TELEPORTBASE_H
#define TELEPORTBASE_H

#include "./Hechizo.h"

class BaseT;

class TeleportBase: public Hechizo{
public:
	TeleportBase(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	~TeleportBase();
	void Lanzar(Player* p);
	void DieReset();
	void WasteMana(Player* p, float deltaTime);
	bool CheckMP(float MP);

protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
	bool 	justPlaced;	// Se acaba de poner? Si/No
	BaseT* 	base; 		// Bloque que hará de aspecto visual para el teletransporte

	SoundEvent* spawnEvent;
};

#endif