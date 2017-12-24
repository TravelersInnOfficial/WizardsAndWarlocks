#ifndef HASARRIVED_H
#define HASARRIVED_H

#include <iostream>

#include "./../Task.h"

class HasArrived: public Task{
public:
	HasArrived();
	bool run(Blackboard* bb);
private:
	float arrivedTarget;
};

#endif