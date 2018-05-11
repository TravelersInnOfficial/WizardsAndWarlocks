#ifndef SUPERSPEED_H
#define SUPERSPEED_H

#include "./Hechizo.h"

class Superspeed: public Hechizo{
public:
	Superspeed(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
};

#endif