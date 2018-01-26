#ifndef PURSUE_H
#define PURSUE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

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