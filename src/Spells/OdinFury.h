#ifndef ODINFURY_H
#define ODINFURY_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class OdinFury: public Hechizo{
public:
	OdinFury(int costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
private:
	
};

#endif