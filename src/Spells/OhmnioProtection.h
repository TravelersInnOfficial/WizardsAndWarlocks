#ifndef OHMNIOPROTECTION_H
#define OHMNIOPROTECTION_H

#include "./Hechizo.h"

#include <iostream>

class OhmnioProtection: public Hechizo{
public:
	OhmnioProtection(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
};

#endif