#ifndef T_WANDER_H
#define T_WANDER_H

#include <iostream>

#include "./../Task.h"

class T_Wander: public Task{
public:
	T_Wander();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

#endif