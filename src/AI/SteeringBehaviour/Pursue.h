#ifndef PURSUE_H
#define PURSUE_H

#include "Seek.h"

class Pursue: public Seek{
public:
	Pursue();
	~Pursue();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);

private:
	float maxPrediction;
};

#endif