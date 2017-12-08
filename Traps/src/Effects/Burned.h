#ifndef BURNED_H
#define BURNED_H

#include "./Effect.h"

class Burned: public Effect{
public:
	Burned(float time, int d);
	void UpdateEffect(Player* p);
private:
	int damage;
};

class SlowedDown: public Effect{
public:
	SlowedDown(float time, float value);
	void ApplyEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	float factor;
};

#endif