#include "ManagerEffect.h"
#include "./../Game.h"

ManagerEffect* ManagerEffect::instance = 0;

ManagerEffect::ManagerEffect(){
	maxTime = 0.5f;
	currentTime = 0.0f;
}

ManagerEffect::~ManagerEffect(){
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

ManagerEffect* ManagerEffect::GetInstance(){
	if(instance==0){
		instance = new ManagerEffect();
	}
	return instance;
}

void ManagerEffect::AddEffect(Player* p, EFFECTCODE EFFECT){
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

void ManagerEffect::CleanEffects(Player* p){
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

void ManagerEffect::UpdateEffects(){
	float deltaTime = Game::GetInstance()->GetDeltaTime();
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

Effect* ManagerEffect::CreateEffect(EFFECTCODE EFFECT){
	Effect * e;
	switch(EFFECT){
		case EFFECT_BASIC:
			e = new Effect(5.0f);
			break;
		case EFFECT_BURNED:
			e = new Burned(6.0f, 10);
			break;
		case EFFECT_SLOWEDDOWN:
			e = new SlowedDown(6.0f, 10.0f);
			break;
		default:
			e = new Effect(5.0f);
	}
	return e;
}
