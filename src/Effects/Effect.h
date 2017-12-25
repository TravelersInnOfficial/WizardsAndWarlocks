#ifndef EFFECT_H
#define EFFECT_H

#include "./../Players/Player.h"

class Effect{
public:
	Effect(float time);
	virtual ~Effect();
	bool CheckFinish(float deltaTime);
	virtual void ApplyEffect(Player* p);
	virtual void UpdateEffect(Player* p);
	virtual void RemoveEffect(Player* p);
protected:
	float currentTime;		// Tiempo de duracion del efecto
};

#endif