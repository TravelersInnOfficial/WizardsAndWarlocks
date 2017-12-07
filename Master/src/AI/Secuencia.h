#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Task.h"

using namespace std;
class Secuencia : public Task{
public:
	Secuencia();
	void addChild(Task* task);
	void addChildren(vector<Task*> tasks);
	virtual bool run(Blackboard* bb);
	bool terminate();
protected:
	vector<Task*> children;
private:
	
};

class RandomSecuencia: public Secuencia{
public:
	RandomSecuencia();
	bool run(Blackboard* bb);
	void shuffleVector();
private:
};

#endif
