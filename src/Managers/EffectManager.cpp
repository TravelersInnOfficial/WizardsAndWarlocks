#include "EffectManager.h"
#include <fstream>
#include <json.hpp>

static EffectManager* instance = nullptr;

EffectManager* EffectManager::GetInstance(){
	static EffectManager localInstance;
	if(instance == nullptr){
		localInstance.InitObject();
		instance = &localInstance;
	}
	return instance;
}

EffectManager::EffectManager(){}

EffectManager::~EffectManager(){
	EmptyObject();
}

void EffectManager::InitObject(){
	maxTime = 0.5f;
	currentTime = 0.0f;

	loadEffectsData();
}

void EffectManager::loadEffectsData(){
 	//Takes path from binary location (/bin)
	std::string jsonPath = "./../assets/json/game_data.json";
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	std::map<std::string,EFFECTCODE> codesMap = GetEFFECTCODE_StrMap();
	EFFECTCODE ID;

	for(int i = 0; !j["effects_data"][i].is_null(); i++){
		ID = codesMap[ j["effects_data"][i]["ID"] ];

		effect_name.insert(std::pair<EFFECTCODE,std::string>(ID, j["effects_data"][i]["name"] ));
		effect_time.insert(std::pair<EFFECTCODE,float>(ID, j["effects_data"][i]["duration"] ));
		effect_value.insert(std::pair<EFFECTCODE,float>(ID, j["effects_data"][i]["value"] )); 
	}
}

void EffectManager::EmptyObject(){
	std::map<Player*, std::vector<Effect*>* >::iterator it = effects.begin();
	for(; it != effects.end(); ++it){				// Recorremos entre todos los efectos
		std::vector<Effect*>* currentV(it->second);	// Pillamos el vector de efectos de cada jugador

		int size = currentV->size();
		for(int i=0; i<size; i++){					// Recorremos todos los efectos del jugador
			Effect* effect = currentV->at(i);
			delete effect;
		}
		currentV->clear();
		delete currentV;
	}
	effects.clear();
	instance = nullptr;
}

void EffectManager::AddEffect(Player* p, EFFECTCODE EFFECT_CODE){

	std::map<Player*, std::vector<Effect*>* >::iterator it;
	it = effects.find(p);
	if(it == effects.end()){
		effects[p] = new std::vector<Effect*>();
	}

	// Comprobamos que el jugador no tenga ya puesto el efecto
	std::vector<Effect*>* currentV = effects.at(p);
	int size = currentV->size();
	for(int i=0; i<size; i++){
		Effect* ef = currentV->at(i);
		if(ef->getCode() == EFFECT_CODE){
			ef->ResetTime();
			return;
		}
	}

	Effect* effect = CreateEffect(EFFECT_CODE);
	effect->ApplyEffect(p);
	currentV->push_back(effect);	
}

void EffectManager::CleanEffects(Player* p){

	std::map<Player*, std::vector<Effect*>* >::iterator it;
	it = effects.find(p);
	if(it != effects.end()){
		std::vector<Effect*>* currentV = effects.at(p);
		int size = currentV->size();
		for(int i=0; i<size; i++){
			Effect* effect = currentV->at(i);
			effect->RemoveEffect(p);
			delete effect;
		}
		currentV->clear();
	}
}

void EffectManager::UpdateEffects(float deltaTime){

	currentTime += deltaTime;

	if(currentTime>=maxTime){
		std::map<Player*, std::vector<Effect*>* >::iterator it = effects.begin();
		for(; it != effects.end(); ++it){				// Recorremos entre todos los efectos
			Player* p = it->first;						// Pillamos el jugador actual
			std::vector<Effect*>* currentV(it->second);	// Pillamos el vector de efectos de cada jugador

			int size = currentV->size();
			for(int i=size-1; i>=0; i--){					// Recorremos todos los efectos del jugador
				Effect* effect = currentV->at(i);
				effect->UpdateEffect(p);
				if(effect->CheckFinish(currentTime)){		// Comprobamos si ha terminado el efecto
					effect->RemoveEffect(p);
					delete effect;
					currentV->erase(currentV->begin()+i);
				}
			}
		}
		currentTime = 0.0f;
	}

	// UPDATE PARTICLES
	std::map<Player*, std::vector<Effect*>* >::iterator it = effects.begin();
	for(; it != effects.end(); ++it){
		Player* p = it->first;
		std::vector<Effect*>* currentV(it->second);

		int size = currentV->size();
		for(int i=size-1; i>=0; i--){
			Effect* effect = currentV->at(i);
			effect->UpdateEffectParticles(p);
		}
	}

}

bool EffectManager::CheckEffect(Player* p, EFFECTCODE EFFECT){
	if(effects.size() != 0){
		std::map<Player*, std::vector<Effect*>* >::iterator it;
		it = effects.find(p);
		if(it != effects.end()){
			std::vector<Effect*>* currentV = effects.at(p);
			
			int size = currentV->size();
			for(int i=0; i<size; i++){							// Recorremos todos los efectos del jugador
				Effect* effect = currentV->at(i);
				if(effect!= nullptr && effect->getCode() == EFFECT)	// Comprobamos que el jugador tiene el efecto adecuado
					return true;
			}
		}
	}
	return false;
}

Effect* EffectManager::CreateEffect(EFFECTCODE EFFECT){
	Effect * e;
	float e_time;
	float e_value;
	switch(EFFECT){
		case WEAK_BURNED:
			e_time = effect_time[WEAK_BURNED];
			e_value = effect_value[WEAK_BURNED];
			e = new Burned(e_time, e_value);
		break;

		case WEAK_SOFTBURNED:
			e_time = effect_time[WEAK_SOFTBURNED];
			e_value = effect_value[WEAK_SOFTBURNED];
			e = new Burned(e_time, e_value);
		break;

		case WEAK_FROZEN:
			e_time = effect_time[WEAK_FROZEN];
			e_value = effect_value[WEAK_FROZEN];
			e = new Frozen(e_time, e_value);
		break;

		case WEAK_PARALYZED:
			e_time = effect_time[WEAK_PARALYZED];
			e_value = effect_value[WEAK_PARALYZED];
			e = new Paralyzed(e_time);
		break;

		case WEAK_SLOWEDDOWN:
			e_time = effect_time[WEAK_SLOWEDDOWN];
			e_value = effect_value[WEAK_SLOWEDDOWN];
			e = new SlowedDown(e_time, e_value);
		break;

		case WEAK_POISONED:
			e_time = effect_time[WEAK_POISONED];
			e_value = effect_value[WEAK_POISONED];
			e = new Poisoned(e_time, e_value);
		break;

		case WEAK_SOFTPOISONED:
			e_time = effect_time[WEAK_SOFTPOISONED];
			e_value = effect_value[WEAK_SOFTPOISONED];
			e = new Poisoned(e_time, e_value);
		break;

		case WEAK_SILENCED:
			e_time = effect_time[WEAK_SILENCED];
			e = new Silenced(e_time);
		break;

		case WEAK_MADNESS:
			e_time = effect_time[WEAK_MADNESS];
			e = new Madness(e_time);
		break;

		case WEAK_DEATHSNARE:
			e_time = effect_time[WEAK_DEATHSNARE];
			e_value = effect_value[WEAK_DEATHSNARE];
			e = new DeathSnare(e_time, e_value);
		break;

		case POWERUP_DAMAGE:
			e_time = effect_time[POWERUP_DAMAGE];
			e_value = effect_value[POWERUP_DAMAGE];
			e = new DamageUp(e_time, e_value);
		break;

		case POWERUP_DEFENSE:
			e_time = effect_time[POWERUP_DEFENSE];
			e_value = effect_value[POWERUP_DEFENSE];
			e = new DefenseUp(7.0f);
		break;

		case WEAK_FUZZY:
			e_time = effect_time[WEAK_FUZZY];
			e = new Fuzzy(e_time);
		break;

		/*
		case POWERUP_ELEMDEFENSE:
			e = new ElementUp(7.0f);
		break;

		*/
		case POWERUP_SPEED:			
			e_time = effect_time[POWERUP_SPEED];
			e_value = effect_value[POWERUP_SPEED];
			e = new SpeedUp(e_time, e_value);
		break;

		case POWERUP_UNTARGET:
			e_time = effect_time[POWERUP_UNTARGET];
			e_value = effect_value[POWERUP_UNTARGET];
			e = new Untargetable(e_time, e_value);
		break;

		case POWERUP_FIRE:
			e_time = effect_time[POWERUP_FIRE];
			e = new FireShots(e_time);
		break;

		case POWERUP_POISON:
			e_time = effect_time[POWERUP_POISON];
			e = new PoisonShots(7.0f);
		break;

		case POWERUP_INVISIBLE:
			e_time = effect_time[POWERUP_INVISIBLE];
			e = new Invisible(e_time);
		break;

		default:
			e_time = effect_time[WEAK_BASIC];
			e = new Effect(5.0f, WEAK_BASIC);
		break;
	}
	return e;
}

std::map<std::string, EFFECTCODE> EffectManager::GetEFFECTCODE_StrMap(){
	std::map<std::string, EFFECTCODE> ret_map;
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_BASIC", WEAK_BASIC));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_BURNED", WEAK_BURNED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_SOFTBURNED", WEAK_SOFTBURNED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_FROZEN", WEAK_FROZEN));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_PARALYZED", WEAK_PARALYZED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_SLOWEDDOWN", WEAK_SLOWEDDOWN));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_POISONED", WEAK_POISONED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_SOFTPOISONED", WEAK_SOFTPOISONED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_SILENCED", WEAK_SILENCED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_GHOSTDISTORSION", WEAK_GHOSTDISTORSION));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_MADNESS", WEAK_MADNESS));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_DEATHSNARE", WEAK_DEATHSNARE));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("WEAK_FUZZY", WEAK_FUZZY));

	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_DAMAGE", POWERUP_DAMAGE));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_DEFENSE", POWERUP_DEFENSE));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_SPEED", POWERUP_SPEED));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_UNTARGET", POWERUP_UNTARGET));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_FIRE", POWERUP_FIRE));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_POISON", POWERUP_POISON));
	ret_map.insert(std::pair<std::string, EFFECTCODE>("POWERUP_INVISIBLE", POWERUP_INVISIBLE));

	return ret_map;
}

std::string EffectManager::GetEffectName(EFFECTCODE effect){
	return effect_name[effect];
}

float EffectManager::GetEffectDuration(EFFECTCODE effect){
	return effect_time[effect];
}

float EffectManager::GetEffectValue(EFFECTCODE effect){
	return effect_value[effect];
}
