#ifndef TELEPORT_H
#define TELEPORT_H

#include "Hechizo.h"
#include "./../Players/Player.h"

class Teleport: public Hechizo{
public:
	Teleport(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
	float distTeleport;
};

#endif