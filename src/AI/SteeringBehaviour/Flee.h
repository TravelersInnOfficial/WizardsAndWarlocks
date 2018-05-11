#ifndef FLEE_H
#define FLEE_H

class SteeringOutput;
class Kinematic;

class Flee{
public:
	Flee();
	~Flee();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float maxAcceleration;
};


#endif