#include "SpellManager.h"
#include "./../GraphicEngine/GraphicEngine.h"


SpellManager* SpellManager::instance = 0;

SpellManager::SpellManager(){
}

SpellManager::~SpellManager(){
	for(int i=0; i<numHechizos; i++){
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){		// Recorremos entre todos los hechizos
			Hechizo* h = it->second;			// Cargamos el hechizo
			delete h;
		}
	}
}

SpellManager* SpellManager::GetInstance(){
	if(instance==0) instance = new SpellManager();
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
bool SpellManager::AddHechizo(int num, Player* p, SPELLCODE type){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		Hechizo* h = hechizos[num][p];			// Nos guardamos el hechizo que habia antes guardado
		if(h!=NULL) delete h;					// En el caso de que ya existiese un Hechizo guardado lo eliminamos
		hechizos[num][p] = CrearHechizo(type);	// Anyadimos el nuevo hechizo
		return true;							// Hechizo asignado
	}
	return false;								// Hechizo no asignado
}

/**
 * @brief [Actualiza los valores de Cooldown de los hechizos]
 * @details [long description]
 */
void SpellManager::UpdateCooldown(float deltaTime){
	this->deltaTime = deltaTime;					// Hacemos update de nuestro deltaTime
	for(int i=0; i<numHechizos; i++){			// Recorremos todos los hashtables que tenemos
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){		// Recorremos entre todos los hechizos
			Hechizo* h = it->second;			// Cargamos el hechizo
			if(h->GetCurrentCooldown()>0){	 	// Comprobamos si esta en cooldown
				h->DecCooldown(this->deltaTime);		// Le pasamos el tiempo que tiene que reducirse el cooldown
			}
		}
	}	
}

bool SpellManager::LanzarHechizo(int num, Player* p){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		Hechizo* h = hechizos[num][p];			// Cargamos el hechizo en una variables
		if(h!=NULL){							// Comprobamos si realmente existe
			if(h->ComprobarCast(deltaTime)){	// Empezamos a Castearlo
				p->ChangeMP(h->GetMP());
				h->Lanzar(p);					// Lanzamos el hechizo
				return true;
			}
		}
	}
	return false;
}

void SpellManager::ResetHechizo(int num, Player* p){
	if(num>=0 && num<numHechizos){				// Comprobamos si el numero de hechizo pasado es correcto
		Hechizo* h = hechizos[num][p];			// Cargamos el hechizo en una variables
		if(h!=NULL){							// Comprobamos si realmente existe
			int mana = p->GetMP();
			if(h->CheckMP(mana)){
				h->EmpezarCast();				// Lanzamos el hechizo
			}
		}
	}
}

Hechizo* SpellManager::CrearHechizo(SPELLCODE type){
	Hechizo* h;
	switch(type){
		case SPELL_BASIC:		// Hechizo instantaneo
			h = new Hechizo(-70, 2.0f, 5.0f);
			break;
		case SPELL_PROYECTIL:	//Hechizo de ataque basico
			h = new HechizoProyectil(-5, 0.0f, 0.25f);
			break;
		default:
			h = NULL;
	}
	return h;
}