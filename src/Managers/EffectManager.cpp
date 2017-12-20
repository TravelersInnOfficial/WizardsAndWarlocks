#include "EffectManager.h"

EffectManager* EffectManager::instance = 0;

EffectManager::EffectManager(){
	maxTime = 0.5f;
	currentTime = 0.0f;
}

EffectManager::~EffectManager(){
	std::map<Player*,vector<Effect*>* >::iterator it = effects.begin();
	for(; it != effects.end(); ++it){				// Recorremos entre todos los efectos
		std::vector<Effect*>* currentV(it->second);	// Pillamos el vector de efectos de cada jugador

		int size = currentV->size();
		for(int i=0; i<size; i++){					// Recorremos todos los efectos del jugador
			Effect* effect = currentV->at(i);
			delete effect;
		}
		delete currentV;
	}
}

EffectManager* EffectManager::GetInstance(){
	if(instance==0){
		instance = new EffectManager();
	}
	return instance;
}

void EffectManager::AddEffect(Player* p, EFFECTCODE EFFECT){
	std::map<Player*, vector<Effect*>* >::iterator it;
	it = effects.find(p);
	if(it == effects.end()){
		effects[p] = new vector<Effect*>();
	}
	vector<Effect*>* currentV = effects.at(p);
	Effect* effect = CreateEffect(EFFECT);
	effect->ApplyEffect(p);
	currentV->push_back(effect);	
}

void EffectManager::CleanEffects(Player* p){
	std::map<Player*, vector<Effect*>* >::iterator it;
	it = effects.find(p);
	if(it != effects.end()){
		vector<Effect*>* currentV = effects.at(p);
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
		std::map<Player*,vector<Effect*>* >::iterator it = effects.begin();
		for(; it != effects.end(); ++it){				// Recorremos entre todos los efectos
			Player* p = it->first;						// Pillamos el jugador actual
			std::vector<Effect*>* currentV(it->second);	// Pillamos el vector de efectos de cada jugador

			int size = currentV->size();
			for(int i=0; i<size; i++){					// Recorremos todos los efectos del jugador
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
}

Effect* EffectManager::CreateEffect(EFFECTCODE EFFECT){
	Effect * e;
	switch(EFFECT){
		case WEAK_BASIC:
			e = new Effect(5.0f);
			break;
		case WEAK_BURNED:
			e = new Burned(6.0f, 5);
			break;
		case WEAK_FROZEN:
			e = new Frozen(6.0f, 10.0f);
			break;
		case WEAK_PARALYZED:
			e = new Paralyzed(2.0f, 1000.0f);
			break;
		case WEAK_SLOWEDDOWN:
			e = new SlowedDown(6.0f, 10.0f);
			break;
		default:
			e = new Effect(5.0f);
			break;
	}
	return e;
}
