#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <iostream>
#include <map>
#include <vector>

#include "./../Effects/Effect.h"
#include "./../Effects/Burned.h"
#include "./../Players/Player.h"
#include <EffectCodes.h>

class EffectManager{
public:
	static EffectManager* GetInstance();
	~EffectManager();

	void AddEffect(Player* p, EFFECTCODE e);
	void CleanEffects(Player* p);
	void UpdateEffects(float deltaTime);
private:
	Effect* CreateEffect(EFFECTCODE e);

	std::map<Player*, vector<Effect*>* > effects;

	float currentTime;
	float maxTime;

	EffectManager();
	static EffectManager* instance;
};

#endif