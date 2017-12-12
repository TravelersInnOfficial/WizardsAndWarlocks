#ifndef FLEE_H
#define FLEE_H

#include <iostream>
#include <vector3d.h>
#include <kinematicTypes.h>

class Flee{
public:
	static SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin, float maxAc);
private:

};


#endif