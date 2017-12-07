#ifndef SEEK_H
#define SEEK_H

#include <iostream>

#include "./../Task.h"

class Seek: public Task{
public:
	Seek();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

#endif