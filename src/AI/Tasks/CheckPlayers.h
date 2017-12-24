#ifndef CHECKPLAYERS_H
#define CHECKPLAYERS_H

#include <iostream>

#include "./../Task.h"

class CheckPlayers: public Task{
public:
	CheckPlayers();
	bool run(Blackboard* bb);
private:
};

#endif