#include "Flee.h"

SteeringOutput Flee::GetSteering(Kinematic cKin, Kinematic tKin, float maxAc){
	SteeringOutput output;

	vector3df linear = cKin.position - tKin.position;

	linear.normalize();
	linear = linear * maxAc;

	output.linear = linear;

	return output;
}