#ifndef EFFECT_H
#define EFFECT_H

#include "./../Player.h"

class Effect{
public:
	Effect();
	~Effect();
	bool CheckFinish(float deltaTime);
	void ApplyEffect(Player* p);
	void UpdateEffect(Player* p);
	void RemoveEffect(Player* p);
private:
	float currentTime;
};

#endif