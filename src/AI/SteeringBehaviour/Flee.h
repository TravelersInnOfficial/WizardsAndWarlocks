#ifndef FLEE_H
#define FLEE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class Flee{
public:
	Flee();
	~Flee();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float maxAcceleration;
};


#endif