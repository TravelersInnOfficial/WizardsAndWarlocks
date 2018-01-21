#ifndef POISONAREA_H
#define POISONAREA_H

#include <iostream>
#include <Alliance.h>

#include "./DamageArea.h"

class PoisonArea: public DamageArea{
public:
	PoisonArea(int dam, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~PoisonArea();

	bool Update(float deltaTime);
	void Contact(void* punt, EntityEnum tipo);

	void SetAlliance(Alliance emi);

protected:
	void CreatePoisonArea(vector3df TPosition, vector3df TScale, vector3df TRotation);

	float 		ghostScale;		// Escala del area de danyo
	Alliance 	emisor;			// Alianza emisora del area
	
};

#endif