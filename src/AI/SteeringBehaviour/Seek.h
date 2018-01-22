#ifndef SEEK_H
#define SEEK_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class Seek{
public:
	Seek();
	~Seek();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
private:
	float maxAcceleration;
};

#endif