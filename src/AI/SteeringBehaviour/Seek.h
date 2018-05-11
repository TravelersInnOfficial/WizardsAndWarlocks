#ifndef SEEK_H
#define SEEK_H

class SteeringOutput;
class Kinematic;

class Seek{
public:
	Seek();
	~Seek();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float maxAcceleration;
};

#endif