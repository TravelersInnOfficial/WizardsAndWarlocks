#ifndef VELOCITYMATCH_H
#define VELOCITYMATCH_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class VelocityMatch{
public:
	static SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin, float maxAcc);
private:
	static float timeToTarget;
};


#endif