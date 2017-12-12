#ifndef PURSUE_H
#define PURSUE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

#include "Seek.h"

class Pursue: public Seek{
public:
	static SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin, float maxAcc);

private:
	static float maxPrediction;
};

#endif