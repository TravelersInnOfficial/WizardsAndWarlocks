#include "ControllerPlayer.h"

Key_player::Key_player(TKEY_CODE k, ACTION_ENUM a){
	key = k;
	status = UP;
	action = a;
}

void Key_player::SetStatus(keyStatesENUM k){
	status = k;
}

TKEY_CODE Key_player::GetKey(){
	return key;
}

ACTION_ENUM Key_player::GetAction(){
	return action;
}

keyStatesENUM Key_player::GetStatus(){
	return status;
}

Key_player::~Key_player(){
}


//-----------------------------------------------------------------------------------------------

ControllerPlayer::~ControllerPlayer(){
	int size = keys.size();
	for(int i = 0; i<size; i++){
		Key_player* k = keys[i];
		delete k;
	}
	keys.clear();
}

ControllerPlayer::ControllerPlayer(){}

/**
 * Anyade una nueva accion (tecla) al controlador
 * 
 * @key - Tecla de la accion
 * @ac - Nombre de la accion
 * 
 * @return - Si se ha podido crear bien la accion (No permite dos acciones con la misma tecla)
 */
bool ControllerPlayer::AddAction(TKEY_CODE key, ACTION_ENUM ac){
	int size = keys.size();						
	for(int i=0; i<size; i++){					// Recorremos todas las acciones actuales
		Key_player* k = keys[i];		
		if(k->GetKey() == key){					// Comprobamos si la tecla ya esta asignada
			return false;
		}
	}
	keys.push_back(new Key_player(key, ac));	// Si no estaba asignada la anyade
	return true;
}

/**
 * Comprueba si una tecla esta pulsada
 * 
 * @num - Posicion de la tecla en el vector
 * @return  True->Se ha pulsado
 */
bool ControllerPlayer::IsKeyDown(int num){
	if(num < keys.size()){				// Comprueba que el numero no se salga del vector
		Key_player* k = keys[num];
		if(k->GetStatus()==DOWN){		// Comprueba si el Status de la tecla es el correcto
			return true;
		}
	}
	return false;
}

/**
 * Comprueba si una tecla esta pulsada
 * 
 * @param action - Nombre de la accion
 * @return  True->Se ha pulsado
 */
bool ControllerPlayer::IsKeyDown(ACTION_ENUM action){
	int size = keys.size();
	for(int i=0; i<size; i++){				// Recorremos todos el vector de teclas
		Key_player* k = keys[i];
		if(action == k->GetAction()){	// Miramos si se corresponde con la accion pasada
			if(k->GetStatus()==DOWN){		// Comprobamos que el Status sea el correspondiente
				return true;
			}
			return false;
		}
	}
	return false;
}

/**
 * Comprueba si una tecla esta siendo pulsada
 * 
 * @num - Posicion de la tecla n el vector
 * @return  True->La tecla esta siendo pulsada
 */
bool ControllerPlayer::IsKeyPressed(int num){
	if(num < keys.size()){
		Key_player* k = keys[num];
		if(k->GetStatus()==DOWN || k->GetStatus()==PRESSED || k->GetStatus()==RELEASED){
			return true;
		}
	}
	return false;
}

/**
 * Comprueba si una tecla esta siendo pulsada
 * 
 * @action - Nombre de la accion
 * @return  True->La tecla esta siendo pulsada
 */
bool ControllerPlayer::IsKeyPressed(ACTION_ENUM action){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(action == k->GetAction()){
			if(k->GetStatus()==DOWN || k->GetStatus()==PRESSED || k->GetStatus()==RELEASED){
				return true;
			}
			return false;
		}
	}
	return false;
}

/**
 * Actualizacion de los Status de las teclas del vector
 */
void ControllerPlayer::Update(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		keyStatesENUM status;
		if(k->GetKey() < 2){
			status = g_engine->GetMouseStatus(k->GetKey());
		} else {
			status = g_engine->GetKeyStatus(k->GetKey());
		}
		k->SetStatus(status);
	}
}

