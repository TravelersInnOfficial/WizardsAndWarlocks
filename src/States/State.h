#ifndef STATE_H
#define STATE_H

#include <iostream>

class State{
public:
	State();
	virtual ~State();

	virtual bool Input();
	virtual void Update(float deltaTime = 0.0f);
	virtual void Draw();
private:
};

#endif