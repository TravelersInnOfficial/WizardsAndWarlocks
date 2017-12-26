#ifndef GUIVERNUMVENTUS_H
#define GUIVERNUMVENTUS_H

#include <iostream>
#include <math.h>

#include "Hechizo.h"
#include "./../Objects/DamageArea.h"

class GuivernumVentus: public Hechizo{
public:
	GuivernumVentus(int costPM, float tCast, float tCoolDown);
	void Lanzar(Player* p);
	void ResetSpell();
private:
	void CreateArea(Player* p);
	void UpdateArea(Player* p);
	
	DamageArea* area;
};


#endif