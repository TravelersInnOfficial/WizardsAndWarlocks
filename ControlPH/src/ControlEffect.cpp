#include "ControlEffect.h"

ControlEffect* ControlEffect::instance = 0;

ControlEffect::ControlEffect(){
	timeStart = 0.0f;
	deltaTime = 0.0f;
}

ControlEffect::~ControlEffect(){
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

ControlEffect* ControlEffect::GetInstance(){
	if(instance==0){
		instance = new ControlEffect();
	}
	return instance;
}

void ControlEffect::AddEffect(Player* p, EFFECTCODE EFFECT){
	std::map<Player*, vector<Effect*>* >::iterator it;
	it = effects.find(p);
	if(it == effects.end()){
		effects[p] = new vector<Effect*>();
	}
	vector<Effect*>* currentV = effects.at(p);
	currentV->push_back(CreateEffect(EFFECT));	
}

void ControlEffect::UpdateEffects(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;

	std::map<Player*,vector<Effect*>* >::iterator it = effects.begin();
	for(; it != effects.end(); ++it){				// Recorremos entre todos los efectos
		Player* p = it->first;						// Pillamos el jugador actual
		std::vector<Effect*>* currentV(it->second);	// Pillamos el vector de efectos de cada jugador

		int size = currentV->size();
		for(int i=0; i<size; i++){					// Recorremos todos los efectos del jugador
			Effect* effect = currentV->at(i);
			if(effect->CheckFinish(deltaTime)){		// Comprobamos si ha terminado el efecto
				effect->RemoveEffect(p);
			}
			else{
				effect->UpdateEffect(p);
			}
		}
	}
}

Effect* ControlEffect::CreateEffect(EFFECTCODE EFFECT){
	Effect * e;
	switch(EFFECT){
		case EFFECT_BASIC:
			e = new Effect();
			break;
	}
	return e;
}

void ControlEffect::StartTime(){
	timeStart = GraphicEngine::getInstance()->getTime();
}