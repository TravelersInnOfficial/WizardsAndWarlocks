#ifndef TASK_H
#define TASK_H

#include "Blackboard.h"

#include <iostream>
using namespace std;
class Task{
public:
	virtual bool run(Blackboard* bb);
	bool terminate();
	virtual ~Task();

private:
};

#endif


