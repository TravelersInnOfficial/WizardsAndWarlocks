#ifndef STATE_H
#define STATE_H

#include <iostream>

class State{
public:
	State();
	~State();

	virtual bool Input();
	virtual void Update();
	virtual void Draw();
private:
};

#endif