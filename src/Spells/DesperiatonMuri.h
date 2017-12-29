#ifndef DESPERIATONMURI_H
#define DESPERIATONMURI_H

#include <iostream>
#include <math.h>

#include "Hechizo.h"
#include "./../Players/Player.h"

class DesperiatonMuri: public Hechizo{
public:
	DesperiatonMuri(int costPM, float tCast, float tCoolDown);
	void Lanzar(Player* p);
private:
	
};

#endif