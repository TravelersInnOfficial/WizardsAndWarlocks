#ifndef ARRIVE_H
#define ARRIVE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class Arrive{
public:
	Arrive();
	~Arrive();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float slowRadius;
	float targetRadius;
	float maxSpeed;
	float timeToTarget;
	float maxAcceleration;
};

#endif