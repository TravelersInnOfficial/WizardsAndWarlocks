#ifndef INVISIBILITYCAPE_H
#define INVISIBILITYCAPE_H

#include "Hechizo.h"

class InvisibilityCape : public Hechizo{
public:
    InvisibilityCape(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();    
};

#endif