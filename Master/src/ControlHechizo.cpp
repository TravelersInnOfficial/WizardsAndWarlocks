#include "ControlHechizo.h"

ControlHechizo* ControlHechizo::instance = 0;

ControlHechizo::ControlHechizo(){}

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
bool ControlHechizo::AddHechizo(int num, Player* p){
	// Comprobamos si el numero de hechizo pasado es correcto
	if(num>=0 && num<numHechizos){
		// Nos guardamos el hechizo que habia antes guardado
		Hechizo* h = hechizos[num][p];
		if(h!=NULL){
			// En el caso de que ya existiese un Hechizo guardado lo eliminamos
			delete h;
		}
		// Anyadimos el nuevo hechizo
		hechizos[num][p] = CrearHechizo();
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
	// Recorremos todos los hashtables que tenemos
	for(int i=0; i<numHechizos; i++){
		// Recorremos entre todos los hechizos
		std::map<Player*, Hechizo*>::iterator it = hechizos[i].begin();
		for(; it!=hechizos[i].end(); ++it){
			Hechizo* h = it->second;
			// Comprobamos si esta en cooldown
			if(h->GetCurrentCooldown()>0){
				// Le pasamos el tiempo que tiene que reducirse el cooldown
				h->DecCooldown(2.0f);
			}
		}
	}	
}

bool ControlHechizo::LanzarHechizo(int num, Player* p){
	// Comprobamos si el numero de hechizo pasado es correcto
	if(num>=0 && num<numHechizos){
		Hechizo* h = hechizos[num][p];
		if(h!=NULL){
			if(h->ComprobarCast()){
				h->Lanzar(p);
				return true;
			}
		}
	}
	return false;
}

Hechizo* ControlHechizo::CrearHechizo(){
	Hechizo* h = new Hechizo();
	return h;
}