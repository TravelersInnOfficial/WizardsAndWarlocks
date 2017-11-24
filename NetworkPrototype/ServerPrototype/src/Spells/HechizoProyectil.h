#ifndef HECHIZOPROYECTIL_H
#define HECHIZOPROYECTIL_H

#include "Hechizo.h"
#include "./../Player.h"
#include "./../Bullets/BulletCodes.h"

class HechizoProyectil: public Hechizo{
public:
	HechizoProyectil();
	void Lanzar(Player* p);
private:
	
};

#endif