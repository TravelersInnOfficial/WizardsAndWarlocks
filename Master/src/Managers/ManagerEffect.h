#ifndef MANAGEREFFECT_H
#define MANAGEREFFECT_H

#include <iostream>
#include <map>
#include <vector>

#include "./../Effects/Effect.h"
#include "./../Effects/Burned.h"
#include "./../Player.h"
#include <EffectCodes.h>

class ManagerEffect{
public:
	static ManagerEffect* GetInstance();
	~ManagerEffect();

	void AddEffect(Player* p, EFFECTCODE e);
	void CleanEffects(Player* p);
	void UpdateEffects();
private:
	Effect* CreateEffect(EFFECTCODE e);

	std::map<Player*, vector<Effect*>* > effects;

	float currentTime;
	float maxTime;

	ManagerEffect();
	static ManagerEffect* instance;
};

#endif