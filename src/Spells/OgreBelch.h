#ifndef OGREBELCH_H
#define OGREBELCH_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class OgreBelch: public Hechizo{
public:
	OgreBelch(int costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
private:
	
};

#endif