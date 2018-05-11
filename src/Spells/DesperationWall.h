#ifndef DesperationWall_H
#define DesperationWall_H

#include "Hechizo.h"

class DesperationWall: public Hechizo{
public:
	DesperationWall(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	~DesperationWall();
	void Lanzar(Player* p);

protected:	
	void createSoundEvent();
private:
	SoundEvent* spawnEvent;		//Invocation sound event
};

#endif