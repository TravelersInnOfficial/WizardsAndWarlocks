#ifndef OBSTACLEAVOIDANCE_H
#define OBSTACLEAVOIDANCE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>
#include <limits>

#include "Seek.h"

class ObstacleAvoidance: public Seek{
public:
	static SteeringOutput GetSteering(Kinematic cKin);
private:
	static float avoidDistance;
	static float lookAHead;
	static float lookAHead2;
};

#endif