#ifndef ALIGN_H
#define ALIGN_H

#include <iostream>
#include <vector3d.h>
#include <vector2d.h>
#include <kinematicTypes.h>

class Align{
public:
	static SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	static float GetAngular(float angle, float angleSize, float rotC);

	static float maxAngularAcceleration;
	static float maxRotation;
	static float targetRadius;
	static float slowRadius;
	static float timeToTarget;
};

#endif