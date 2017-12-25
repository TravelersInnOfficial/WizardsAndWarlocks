#ifndef DESPERIATONMURI_H
#define DESPERIATONMURI_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class DesperiatonMuri: public Hechizo{
public:
	DesperiatonMuri(int costPM, float tCast, float tCoolDown);
	void Lanzar(Player* p);
private:
	
};

#endif