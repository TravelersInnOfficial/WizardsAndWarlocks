#include "ControlEffect.h"

ControlEffect* ControlEffect::instance = 0;

ControlEffect::ControlEffect(){
	timeStart = 0.0f;
	currentTime = 0.0f;
	maxTime = 0.5f;
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
	Effect* effect = CreateEffect(EFFECT);
	effect->ApplyEffect(p);
	currentV->push_back(effect);	
}

void ControlEffect::UpdateEffects(){
	float deltaTime = GraphicEngine::getInstance()->getTime() * 0.001;
	currentTime += deltaTime - timeStart;
	timeStart = deltaTime;
	//std::cout<<"current:"<<currentTime<<" delta:"<<deltaTime<<std::endl;

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

Effect* ControlEffect::CreateEffect(EFFECTCODE EFFECT){
	Effect * e;
	switch(EFFECT){
		case EFFECT_BASIC:
			e = new Effect(5.0f);
			break;
		case EFFECT_BURNED:
			e = new Burned(6.0f, 10);
		case EFFECT_SLOWEDDOWN:
			e = new SlowedDown(6.0f, 10.0f);
	}
	return e;
}

void ControlEffect::StartTime(){
	timeStart = GraphicEngine::getInstance()->getTime() * 0.001;
}