#ifndef HECHIZOPROYECTIL_H
#define HECHIZOPROYECTIL_H

#include "Hechizo.h"
#include "./../Players/Player.h"
#include <BulletCodes.h>

class HechizoProyectil: public Hechizo{
public:
	HechizoProyectil(int costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
private:
	
};

#endif