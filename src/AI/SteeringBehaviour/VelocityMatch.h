#ifndef VELOCITYMATCH_H
#define VELOCITYMATCH_H

class SteeringOutput;
class Kinematic;

class VelocityMatch{
public:
	VelocityMatch();
	~VelocityMatch();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float timeToTarget;
	float maxAcceleration;
};


#endif