#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <vector>
#include <stdlib.h>
#include "Task.h"

class Secuencia : public Task{
public:
	Secuencia();
	virtual ~Secuencia();
	void addChild(Task* task);
	void addChildren(std::vector<Task*> tasks);
	virtual bool run(Blackboard* bb);
	bool terminate();
protected:
	std::vector<Task*> children;
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
