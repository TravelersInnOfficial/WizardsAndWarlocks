#ifndef INVOCATION_WALL_H
#define INVOCATION_WALL_H

#include "./Invocation.h"

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class InvocationWall: public Invocation{
public:
	InvocationWall(int HP, float time, vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool Update(float deltaTime);
private:
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);

	float 		currentTime;				// Tiempo 
};


#endif