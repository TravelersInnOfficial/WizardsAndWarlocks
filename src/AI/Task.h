#ifndef TASK_H
#define TASK_H

#include "Blackboard.h"

#include <iostream>

class Task{
public:
	virtual ~Task();
	virtual bool run(Blackboard* bb) = 0;
	bool terminate();
private:
};

#endif


