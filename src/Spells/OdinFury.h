#ifndef ODINFURY_H
#define ODINFURY_H

#include "Hechizo.h"

class OdinFury: public Hechizo{
public:
	OdinFury(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();

private:
	
};

#endif