#ifndef ARRIVE_H
#define ARRIVE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class Arrive{
public:
	static SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin, float maxAcc);
private:
	static float slowRadius;
	static float targetRadius;
	static float maxSpeed;
	static float timeToTarget;
};

#endif