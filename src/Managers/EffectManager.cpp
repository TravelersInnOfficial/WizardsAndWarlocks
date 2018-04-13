#include "EffectManager.h"

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
	switch(EFFECT){
		case WEAK_BURNED:
			e = new Burned(6.0f, 3.0f);
		break;

		case WEAK_SOFTBURNED:
			e = new Burned(3.0f, 3.0f);
		break;

		case WEAK_FROZEN:
			e = new Frozen(3.0f, 0.0f);
		break;

		case WEAK_PARALYZED:
			e = new Paralyzed(3.0f);
		break;

		case WEAK_SLOWEDDOWN:
			e = new SlowedDown(4.0f, 10.0f);
		break;

		case WEAK_POISONED:
			e = new Poisoned(6.0f, 3.0f);
		break;

		case WEAK_SOFTPOISONED:
			e = new Poisoned(3.0f, 3.0f);
		break;

		case WEAK_SILENCED:
			e = new Silenced(4.0f);
		break;

		case WEAK_MADNESS:
			e = new Madness(4.0f);
		break;

		case WEAK_DEATHSNARE:
			e = new DeathSnare(2.0f, 4);
		break;

		case POWERUP_DAMAGE:
			e = new DamageUp(7.0f);
		break;

		case POWERUP_DEFENSE:
			e = new DefenseUp(7.0f);
		break;

		case WEAK_FUZZY:
			e = new Fuzzy(5.0f);
		break;

		/*
		case POWERUP_ELEMDEFENSE:
			e = new ElementUp(7.0f);
		break;

		*/
		case POWERUP_SPEED:
			e = new SpeedUp(4.0f);
		break;

		case POWERUP_UNTARGET:
			e = new Untargetable(4.0f);
		break;

		case POWERUP_FIRE:
			e = new FireShots(7.0f);
		break;

		case POWERUP_POISON:
			e = new PoisonShots(7.0f);
		break;

		case POWERUP_INVISIBLE:
			e = new Invisible(10.0f);
		break;

		default:
			e = new Effect(5.0f, WEAK_BASIC);
		break;
	}
	return e;
}
