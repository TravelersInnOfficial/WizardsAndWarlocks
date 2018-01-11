#ifndef DRAGONBREATH_H
#define DRAGONBREATH_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class DragonBreath: public Hechizo{
public:
	DragonBreath(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
	
};

#endif