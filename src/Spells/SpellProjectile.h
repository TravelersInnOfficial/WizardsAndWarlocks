#ifndef SpellProjectile_H
#define SpellProjectile_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class SpellProjectile: public Hechizo{
public:
	SpellProjectile(int costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
private:
	
};

#endif