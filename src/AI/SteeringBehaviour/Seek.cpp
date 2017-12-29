#include "Seek.h"

float Seek::maxAcceleration = 30.0f;

SteeringOutput Seek::GetSteering(Kinematic cKin, Kinematic tKin){
	SteeringOutput output;

	vector3df linear = tKin.position - cKin.position;

	linear.normalize();
	linear = linear * maxAcceleration;

	output.linear = linear;

	return output;
}