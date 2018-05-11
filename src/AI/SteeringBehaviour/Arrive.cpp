#include "Arrive.h"

#include <kinematicTypes.h>

Arrive::Arrive(){
	slowRadius = 3.0f;
	targetRadius = 0.5f;
	maxSpeed = 30.0f;
	timeToTarget = 0.1f;
	maxAcceleration = 30.0f;
}

Arrive::~Arrive(){}

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