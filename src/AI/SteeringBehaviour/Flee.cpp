#include "Flee.h"

float Flee::maxAcceleration = 30.0f;

SteeringOutput Flee::GetSteering(Kinematic cKin, Kinematic tKin){
	SteeringOutput output;

	vector3df linear = cKin.position - tKin.position;

	linear.normalize();
	linear = linear * maxAcceleration;

	output.linear = linear;

	return output;
}