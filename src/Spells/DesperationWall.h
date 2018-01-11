#ifndef DesperationWall_H
#define DesperationWall_H

#include <iostream>
#include <math.h>

#include "Hechizo.h"
#include "./../Players/Player.h"

class DesperationWall: public Hechizo{
public:
	DesperationWall(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	
	//Sound Functions
	void createSoundEvent();

private:
	
};

#endif