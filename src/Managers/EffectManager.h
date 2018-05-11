#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <iostream>
#include <vector>
#include <map>

#include <EffectCodes.h>

class Player;
class Effect;

class EffectManager{
public:
	static EffectManager* GetInstance();
	~EffectManager();
	void InitObject();
	void EmptyObject();

	void AddEffect(Player* p, EFFECTCODE e);
	void CleanEffects(Player* p);
	void UpdateEffects(float deltaTime);
	bool CheckEffect(Player* p, EFFECTCODE e);

	std::map<std::string, EFFECTCODE> GetEFFECTCODE_StrMap();
	std::string GetEffectName(EFFECTCODE);
	float GetEffectDuration(EFFECTCODE);
	float GetEffectValue(EFFECTCODE);

private:
	Effect* CreateEffect(EFFECTCODE e);

	std::map<Player*, std::vector<Effect*>* > effects;
	std::map<EFFECTCODE, std::string> effect_name;
	std::map<EFFECTCODE, float> effect_time;
	std::map<EFFECTCODE, float> effect_value;

	float currentTime;
	float maxTime;

	EffectManager();
	EffectManager(EffectManager&);
	EffectManager operator =(EffectManager&);
	void loadEffectsData();
};

#endif