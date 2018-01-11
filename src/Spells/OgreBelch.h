#ifndef OGREBELCH_H
#define OGREBELCH_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class OgreBelch: public Hechizo{
public:
	OgreBelch(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
	
protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
	
};

#endif