#include "Seek.h"

Seek::Seek(){
	maxAcceleration = 30.0f;
}

Seek::~Seek(){}

SteeringOutput Seek::GetSteering(Kinematic cKin, Kinematic tKin){
	SteeringOutput output;

	vector3df linear = tKin.position - cKin.position;

	linear.normalize();
	linear = linear * maxAcceleration;

	output.linear = linear;

	return output;
}