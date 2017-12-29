#include "VelocityMatch.h"

float VelocityMatch::timeToTarget = 0.1f;
float VelocityMatch::maxAcceleration = 30.f;

SteeringOutput VelocityMatch::GetSteering(Kinematic cKin, Kinematic tKin){
	SteeringOutput output;

	vector3df linear = tKin.velocity - cKin.velocity;
	linear = linear / timeToTarget;

	if (linear.length() > maxAcceleration){
		linear.normalize();
		linear = linear*maxAcceleration;
	}

	output.linear = linear;

	return output;
}