#ifndef EFFECT_H
#define EFFECT_H

#include "./../Players/Player.h"
#include <EffectCodes.h>

class Effect{
public:
	Effect(float time, EFFECTCODE cod);
	virtual ~Effect();
	bool CheckFinish(float deltaTime);
	virtual void ApplyEffect(Player* p);
	virtual void UpdateEffect(Player* p);
	virtual void RemoveEffect(Player* p);
	EFFECTCODE getCode();
	
protected:
	float currentTime;		// Tiempo de duracion del efecto
	EFFECTCODE code;
};

#endif