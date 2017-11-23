#include "ControlHechizo.h"
#include "./GraphicEngine/GraphicEngine.h"

ControlHechizo* ControlHechizo::instance = 0;

ControlHechizo::ControlHechizo(){
	timeStart = 0.0f;
	deltaTime = 0.0f;
}

ControlHechizo* ControlHechizo::GetInstance(){
	if(instance==0){
		instance = new ControlHechizo();
	}
	return instance;
}

/**
 * @brief [Asignacion de Hechizo y jugador]
 * @details [long description]
 * 
 * @param num 	[numero de hechizo para asignar]
 * @param p 	[jugador al que asignarle el hechizo]
 * 
 * @return 		[Se ha asignado correctamente el hechizo]
 */
bool ControlHechizo::AddHechizo(int num, Player* p, SPELLCODE type){
	// Comprobamos si el numero de hechizo pasado es correcto
	if(num>=0 && num<numHechizos){
		// Nos guardamos el hechizo que habia antes guardado
		Hechizo* h = hechizos[num][p];
		if(h!=NULL){
			// En el caso de que ya existiese un Hechizo guardado lo eliminamos
			delete h;
		}
		// Anyadimos el nuevo hechizo
		hechizos[num][p] = CrearHechizo(type);
		// Hechizo asignado
		return true;
	}
	// Hechizo no asignado
	return false;
}

/**
 * @brief [Actualiza los valores de Cooldown de los hechizos]
 * @details [long description]
 */
void ControlHechizo::UpdateCooldown(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;

	for(int i=0; i<numHechizos; i++){			// Recorremos todos los hashtables que tenemos
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){		// Recorremos entre todos los hechizos
			Hechizo* h = it->second;			// Cargamos el hechizo
			if(h->GetCurrentCooldown()>0){	 	// Comprobamos si esta en cooldown
				h->DecCooldown(deltaTime);		// Le pasamos el tiempo que tiene que reducirse el cooldown
			}
		}
	}	
}

bool ControlHechizo::LanzarHechizo(int num, Player* p){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		Hechizo* h = hechizos[num][p];			// Cargamos el hechizo en una variables
		if(h!=NULL){							// Comprobamos si realmente existe
			if(h->ComprobarCast()){				// Empezamos a Castearlo
				h->Lanzar(p);					// Lanzamos el hechizo
				return true;
			}
		}
	}
	return false;
}

Hechizo* ControlHechizo::CrearHechizo(SPELLCODE type){
	Hechizo* h;
	switch(type){
		case SPELL_BASIC:		// Hechizo instantaneo
			h = new Hechizo();
			break;
		case SPELL_PROYECTIL:	//Hechizo de ataque basico
			h = new HechizoProyectil();
			break;
	}
	return h;
}

void ControlHechizo::StartTime(){
	timeStart = GraphicEngine::getInstance()->getTime();
}