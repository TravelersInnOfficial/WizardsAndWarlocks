#ifndef EFFECT_H
#define EFFECT_H

#include "./../Players/Player.h"
#include <EffectCodes.h>

class Effect{
public:
	Effect(float time, EFFECTCODE cod);
	virtual ~Effect();
	void ResetTime();
	bool CheckFinish(float deltaTime);
	virtual void ApplyEffect(Player* p);
	virtual void UpdateEffect(Player* p);
	virtual void RemoveEffect(Player* p);
	EFFECTCODE getCode();
	
protected:
	float maxTime;			// Tiempo de duracion del efecto
	float currentTime;		// Tiempo actual de duracion del efecto
	EFFECTCODE code;
};

#endif