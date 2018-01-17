#ifndef INVOCATION_WALL_H
#define INVOCATION_WALL_H

#include "./Invocation.h"

#include <iostream>

class InvocationWall: public Invocation{
public:
	InvocationWall(int HP, float time, vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool Update(float deltaTime);
private:
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);
	void createSoundEvent();

	float 		currentTime;				// Tiempo 
};


#endif