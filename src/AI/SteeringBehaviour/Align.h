#ifndef ALIGN_H
#define ALIGN_H

#include <iostream>

class SteeringOutput;
class Kinematic;

class Align{
public:
	Align();
	~Align();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float GetAngular(float angle, float angleSize, float rotC);

	float maxAngularAcceleration;
	float maxRotation;
	float targetRadius;
	float slowRadius;
	float timeToTarget;
};

#endif