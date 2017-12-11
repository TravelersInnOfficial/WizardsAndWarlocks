#include "Pursue.h"

float Pursue::maxPrediction = 1.0f;

SteeringOutput Pursue::GetSteering(Kinematic cKin, Kinematic tKin, float maxAcc){
	vector3df direction = tKin.position - cKin.position;
	float distance = direction.length();

	float speed = cKin.velocity.length();
	float prediction = 0;
	if(speed<=distance/maxPrediction){
		prediction = maxPrediction;
	}else{
		prediction = distance/speed;
	}

	tKin.position = tKin.position + (tKin.velocity*prediction);

	return Seek::GetSteering(cKin, tKin, maxAcc);
}