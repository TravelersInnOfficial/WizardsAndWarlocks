#ifndef SEEK_H
#define SEEK_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class Seek{
public:
	static SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin, float maxAc);
private:

};

#endif