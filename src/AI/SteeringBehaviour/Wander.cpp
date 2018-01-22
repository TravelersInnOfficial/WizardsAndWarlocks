#include "Wander.h"

Wander::Wander(){
	wanderOffset = 1.0f;
	wanderRadius = 2.0f;
	wanderRate = 5.0f;
	wanderOrientation = 0.0f;
	maxAcceleration = 30.0f;
}

Wander::~Wander(){}

SteeringOutput Wander::GetSteering(Kinematic cKin){
	Kinematic tKin;
	SteeringOutput output;
	float ori = cKin.orientation.Y;
	vector3df asVector(sin(ori),0 ,cos(ori));

	Wander::wanderOrientation = wanderOrientation + (rand()%2001 -1000)*0.001*wanderRate; 	//wanderOr += [-1, +1]*wanderRate
	//std::cout<<wanderOrientation<<std::endl;
	vector2df targetOrientation(0, wanderOrientation);
	//std::cout<<targetOrientation<<std::endl;

	vector3df target = cKin.position + vector3df(sin(wanderOrientation)*wanderOffset, 0, cos(wanderOrientation))*wanderOffset;
	target = target + asVector*wanderRadius;
	//std::cout<<target<<std::endl;
	//std::cout<<cKin.position<<std::endl;
	tKin.position = target;
	tKin.orientation = targetOrientation;

	//std::cout<<tKin.orientation<<std::endl;

	output = Face::GetSteering(cKin, tKin);
	//std::cout<<output.angular<<std::endl;
	output.linear = asVector*maxAcceleration;

	//std::cout<<output<<std::endl;
	return output; 
}