#ifndef EFFECT_H
#define EFFECT_H

#include "./../Player.h"

class Effect{
public:
	Effect(float time);
	~Effect();
	bool CheckFinish(float deltaTime);
	virtual void ApplyEffect(Player* p);
	virtual void UpdateEffect(Player* p);
	virtual void RemoveEffect(Player* p);
protected:
	float currentTime;
};

#endif