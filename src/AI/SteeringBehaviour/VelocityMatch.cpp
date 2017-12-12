#include "VelocityMatch.h"

float VelocityMatch::timeToTarget = 0.1f;

SteeringOutput VelocityMatch::GetSteering(Kinematic cKin, Kinematic tKin, float maxAcc){
	SteeringOutput output;

	vector3df linear = tKin.velocity - cKin.velocity;
	linear = linear / timeToTarget;

	if (linear.length() > maxAcc){
		linear.normalize();
		linear = linear*maxAcc;
	}

	output.linear = linear;

	return output;
}