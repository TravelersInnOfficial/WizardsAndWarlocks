#ifndef GuivernoWind_H
#define GuivernoWind_H

#include "Hechizo.h"

class IceArea;

class GuivernoWind: public Hechizo{
public:
	GuivernoWind(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	~GuivernoWind();
	void Lanzar(Player* p);
	void ResetSpell();
	void DieReset();
	
protected:	
	//Sound Functions
	void createSoundEvent();
	void WasteMana(Player* p, float deltaTime);

private:
	void CreateArea(Player* p);
	void UpdateArea(Player* p);
	
	IceArea* 	area;
	float  		dist; 		// distancia a la que aparecera el area

	float 		timeWaste;		// Tiempo que lleba desde la ultima vez que consumio mana
	float		maxTimeWaste;	// Tiempo maximo de consumicion de danyo
};


#endif