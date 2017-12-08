#ifndef HECHIZOPROYECTIL_H
#define HECHIZOPROYECTIL_H

#include "Hechizo.h"
#include "./../Player.h"
#include <BulletCodes.h>

class HechizoProyectil: public Hechizo{
public:
	HechizoProyectil(int costPM, float tCast, float tCoolDown);
	void Lanzar(Player* p);
private:
	
};

#endif