#ifndef OBSTACLEAVOIDANCE_H
#define OBSTACLEAVOIDANCE_H

#include "Seek.h"

class ObstacleAvoidance: public Seek{
public:
	ObstacleAvoidance();
	~ObstacleAvoidance();
	SteeringOutput GetSteering(Kinematic cKin);
private:
	float avoidDistance;
	float lookAHead;
	float lookAHead2;
};

#endif