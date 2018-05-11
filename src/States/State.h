#ifndef STATE_H
#define STATE_H

class State{
public:
	State();
	virtual ~State();

	virtual bool Input() = 0;
	virtual void Update(float deltaTime = 0.0f) = 0;
	virtual void Draw() = 0;
private:
};

#endif