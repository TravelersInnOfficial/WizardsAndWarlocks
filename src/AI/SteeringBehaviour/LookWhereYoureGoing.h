#ifndef LOOKWHEREYOUREGOING_H
#define LOOKWHEREYOUREGOING_H

#include <iostream>
#include <kinematicTypes.h>

#include "Align.h"

class LookWhereYoureGoing: public Align{
public:
	static SteeringOutput GetSteering(Kinematic cKin);
private:
};

#endif