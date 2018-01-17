#ifndef DUMMY_H
#define DUMMY_H

#include "./Invocation.h"

#include <iostream>

class Dummy: public Invocation{
public:
	Dummy(int HP, float time, bool alli, vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool Update(float deltaTime);
private:
	void UpdatePosShape();
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);

	float 		currentTime; 		// Tiempo de duracion maxima del Dummy
	bool 		alliance; 			// De que alianza es? Mago/Brujo
};


#endif