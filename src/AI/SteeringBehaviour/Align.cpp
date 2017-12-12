#include "Align.h"

	float Align::maxAngularAcceleration = 3.0f;
	float Align::maxRotation = 7.0f;
	float Align::targetRadius = 0.01;
	float Align::slowRadius = M_PI;
	float Align::timeToTarget = 1.0f;

SteeringOutput Align::GetSteering(Kinematic cKin, Kinematic tKin){

	vector2df rotation = tKin.orientation - cKin.orientation;

	rotation.mapToRangePI();
	vector2df rotationSize = rotation.absolute();

	SteeringOutput output;
	output.angular = vector2df(
						Align::GetAngular(rotation.X, rotationSize.X, cKin.rotation.X),
						Align::GetAngular(rotation.Y, rotationSize.Y, cKin.rotation.Y)
					);

	return output;
}

float Align::GetAngular(float angle, float angleSize, float rotC){
	if(angleSize < targetRadius){
		return 0.0f;
	}

	float targetRotation = 0;
	if(angleSize>slowRadius){
		targetRotation = maxRotation;
	}else{
		targetRotation = maxRotation * angleSize/slowRadius;
	}

	targetRotation *= angle/angleSize;
	float angular = targetRotation - rotC;
	angular /= timeToTarget;

	float angularAcceleration = abs(angular);
	if(angularAcceleration > maxAngularAcceleration){
		angular /= angularAcceleration;
		angular *= maxAngularAcceleration;
	}

	return angular;
}