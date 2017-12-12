#include "Seek.h"

SteeringOutput Seek::GetSteering(Kinematic cKin, Kinematic tKin, float maxAc){
	SteeringOutput output;

	vector3df linear = tKin.position - cKin.position;

	linear.normalize();
	linear = linear * maxAc;

	output.linear = linear;

	return output;
}