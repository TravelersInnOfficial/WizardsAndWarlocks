#ifndef SPELLDUMMY_H
#define SPELLDUMMY_H

#include <iostream>
#include <Alliance.h>
#include <InvocationCodes.h>

#include "./Hechizo.h"

class SpellDummy: public Hechizo{
public:
	SpellDummy(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);

protected:	

private:
};


#endif
