#include "VelocityMatch.h"

VelocityMatch::VelocityMatch(){
	timeToTarget = 0.1f;
	maxAcceleration = 30.0f;
}

VelocityMatch::~VelocityMatch(){}

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