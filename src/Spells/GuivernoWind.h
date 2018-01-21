#ifndef GuivernoWind_H
#define GuivernoWind_H

#include <iostream>
#include <AreaCodes.h>

#include "Hechizo.h"
#include "./../DamageAreas/IceArea.h"

class GuivernoWind: public Hechizo{
public:
	GuivernoWind(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	void Lanzar(Player* p);
	void ResetSpell();
	void DieReset();
	
protected:	
	//Sound Functions
	void createSoundEvent();

private:
	void CreateArea(Player* p);
	void UpdateArea(Player* p);
	
	IceArea* area;
	float  		dist; 		// distancia a la que aparecera el area
};


#endif