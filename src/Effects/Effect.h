#ifndef EFFECT_H
#define EFFECT_H

#include <EffectCodes.h>

class Player;
class SoundEvent;

class Effect{
public:
	Effect(float time, EFFECTCODE cod);
	virtual ~Effect();
	void ResetTime();
	bool CheckFinish(float deltaTime);
	virtual void ApplyEffect(Player* p);
	virtual void UpdateEffect(Player* p);
	virtual void RemoveEffect(Player* p);
	virtual void UpdateEffectParticles(Player* p);
	EFFECTCODE getCode();

	//Sound Functions
	SoundEvent* effectEvent;
	void playEffectEvent();
	
protected:
	Player* myPlayer;
	float maxTime;			// Tiempo de duracion del efecto
	float currentTime;		// Tiempo actual de duracion del efecto
	EFFECTCODE code;
	void createSoundEvent();
};

#endif