#ifndef ARRIVE_H
#define ARRIVE_H

class SteeringOutput;
class Kinematic;

class Arrive{
public:
	Arrive();
	~Arrive();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float slowRadius;
	float targetRadius;
	float maxSpeed;
	float timeToTarget;
	float maxAcceleration;
};

#endif