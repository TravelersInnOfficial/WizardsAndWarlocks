#ifndef DUMMY_H
#define DUMMY_H

#include "./Invocation.h"

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Dummy: public Invocation{
public:
	Dummy(int HP, float time, bool alli, vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool Update(float deltaTime);
	void SetName(std::string name);

private:
	void UpdatePosShape();
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);

	float 		currentTime; 		// Tiempo de duracion maxima del Dummy
	bool 		alliance; 			// De que alianza es? Mago/Brujo
};


#endif