#ifndef WANDER_H
#define WANDER_H

#include <iostream>
#include <stdlib.h>
#include <vector3d.h>
#include <vector2d.h>
#include <kinematicTypes.h>

#include "Face.h"

class Wander: public Face{
public:
	Wander();
	~Wander();
	SteeringOutput GetSteering(Kinematic cKin);
private:
	float wanderOffset;			// Holds the radius and forward offset of the wander circle
	float wanderRadius;			
	float wanderRate;			// Holds the maximum rate at which the wander orientation can change
	float wanderOrientation;	// Holds the maximum acceleration of the character
	float maxAcceleration;		// Holds the maximum acceleration of the character
};


#endif