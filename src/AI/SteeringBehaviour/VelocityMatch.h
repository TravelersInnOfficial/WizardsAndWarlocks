#ifndef VELOCITYMATCH_H
#define VELOCITYMATCH_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class VelocityMatch{
public:
	VelocityMatch();
	~VelocityMatch();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float timeToTarget;
	float maxAcceleration;
};


#endif