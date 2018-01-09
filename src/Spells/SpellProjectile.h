#ifndef SpellProjectile_H
#define SpellProjectile_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class SpellProjectile: public Hechizo{
public:
	SpellProjectile(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
private:
	//Sound Functions
	void createSoundEvents();
	void playRegularShot(vector3df pos);
	std::map<std::string, SoundEvent*> soundEvents;		//Sound events
};

#endif