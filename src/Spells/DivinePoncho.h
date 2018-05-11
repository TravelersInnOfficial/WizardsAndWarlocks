#ifndef DIVINEPONCHO_H
#define DIVINEPONCHO_H

#include "./Hechizo.h"

class DivinePoncho: public Hechizo{
public:
	DivinePoncho(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();
private:

};

#endif