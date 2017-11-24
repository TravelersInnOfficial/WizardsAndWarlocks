#ifndef CONTROLEFFECT_H
#define CONTROLEFFECT_H

#include <iostream>
#include <map>
#include <vector>

#include "./Effects/Effect.h"
#include "./Effects/Burned.h"
#include "./Player.h"

#include "./Effects/EffectCodes.h"

class ControlEffect{
public:
	static ControlEffect* GetInstance();
	~ControlEffect();

	void AddEffect(Player* p, EFFECTCODE e);
	void UpdateEffects();
	void StartTime();
private:
	Effect* CreateEffect(EFFECTCODE e);

	std::map<Player*, vector<Effect*>* > effects;

	float timeStart;
	float currentTime;
	float maxTime;

	ControlEffect();
	static ControlEffect* instance;
};

#endif