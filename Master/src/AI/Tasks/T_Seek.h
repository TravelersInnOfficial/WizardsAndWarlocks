#ifndef T_SEEK_H
#define T_SEEK_H

#include <iostream>

#include "./../Task.h"

class T_Seek: public Task{
public:
	T_Seek();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

#endif