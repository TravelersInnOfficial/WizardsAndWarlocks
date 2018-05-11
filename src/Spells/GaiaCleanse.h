#ifndef GAIACLEANSE_H
#define GAIACLEANSE_H

#include "./Hechizo.h"

class GaiaCleanse: public Hechizo{
public:
	GaiaCleanse(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();
	
private:
};

#endif