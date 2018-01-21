#ifndef ICEAREA_H
#define ICEAREA_H

#include <iostream>

#include "./DamageArea.h"

class IceArea: public DamageArea{
public:
	IceArea(int dam, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~IceArea();

	bool Update(float deltaTime);

	void Contact(void* punt, EntityEnum tipo);
	void Deactivate();

	void SetEmisor(int emi);

protected:
	void CreateIceArea(vector3df TPosition, vector3df TScale, vector3df TRotation);

	int 		emisor;				// Emisor del area de danyo
	bool 		activated;			// Esta activa? Deberia seguir existiendo? Si/No
	
};

#endif