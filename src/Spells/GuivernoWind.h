#ifndef GuivernoWind_H
#define GuivernoWind_H

#include <iostream>
#include <math.h>

#include "Hechizo.h"
#include "./../Objects/DamageArea.h"

class GuivernoWind: public Hechizo{
public:
	GuivernoWind(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
	void ResetSpell();
private:
	void CreateArea(Player* p);
	void UpdateArea(Player* p);
	
	DamageArea* area;
	float  		dist; 		// distancia a la que aparecera el area
};


#endif