#ifndef SpellProjectile_H
#define SpellProjectile_H

#include "Hechizo.h"

class SpellProjectile: public Hechizo{
public:
	SpellProjectile(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:
	//Sound Functions
	void createSoundEvent();
	
private:
	
};

#endif