#ifndef LOOKWHEREYOUREGOING_H
#define LOOKWHEREYOUREGOING_H

#include "Align.h"

class LookWhereYoureGoing: public Align{
public:
	LookWhereYoureGoing();
	~LookWhereYoureGoing();
	SteeringOutput GetSteering(Kinematic cKin);
private:
};

#endif