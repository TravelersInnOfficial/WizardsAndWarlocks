#include "Arrive.h"

float Arrive::slowRadius = 3.0f;
float Arrive::targetRadius = 0.5f;
float Arrive::maxSpeed = 30.0f;
float Arrive::timeToTarget = 0.1f;
float Arrive::maxAcceleration = 30.0f;

// Valores a probar

SteeringOutput Arrive::GetSteering(Kinematic cKin, Kinematic tKin){

	SteeringOutput output;

	vector3df direction = tKin.position - cKin.position;
	float distance = direction.length();

	if (distance < targetRadius){	//targetRadius
		return output;
	}

	float targetSpeed = 0;
	if (distance > slowRadius){	//slowRadius
		targetSpeed = maxSpeed;	//maxSpeed
	}else{
		targetSpeed = maxSpeed * distance/slowRadius; //maxSpeed * distance/slowRadius
	}

	vector3df targetVelocity = direction;
	targetVelocity.normalize();
	targetVelocity = targetVelocity * targetSpeed;

	vector3df linear = targetVelocity - cKin.velocity; //targetVelocity
	linear = linear / timeToTarget;	//timeToTarget

	if(linear.length() > maxAcceleration){	// maxAcc
		linear.normalize();
		linear = linear * maxAcceleration;
	}

	output.linear = linear;

	return output;
}