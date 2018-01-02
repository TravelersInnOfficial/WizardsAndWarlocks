#include "PlayerController.h"

Key_player::Key_player(TKEY_CODE k, ACTION_ENUM a){
	key = k;
	status = UP;
	action = a;
}

void Key_player::SetStatus(keyStatesENUM k){
	status = k;
}

void Key_player::SetAction(ACTION_ENUM act){
	action = act;
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

PlayerController::~PlayerController(){
	int size = keys.size();
	for(int i = 0; i<size; i++){
		Key_player* k = keys[i];
		delete k;
	}
	keys.clear();
}

PlayerController::PlayerController(){}

/**
 * Anyade una nueva accion (tecla) al controlador
 * 
 * @key - Tecla de la accion
 * @ac - Nombre de la accion
 * 
 * @return - Si se ha podido crear bien la accion (No permite dos acciones con la misma tecla)
 */
bool PlayerController::AddAction(TKEY_CODE key, ACTION_ENUM ac){
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
 * Pone el status de una accion
 * 
 * @ac - Nombre de la accion
 * @st - Status de la accion
 * 
 * @return - True->Se ha cambiado el status
 */
bool PlayerController::SetStatus(ACTION_ENUM ac, keyStatesENUM st){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(k->GetAction() == ac){
			k->SetStatus(st);
			return true;
		}
	}
	return false;
}

/**
 * Comprueba si una tecla esta pulsada
 * 
 * @num - Posicion de la tecla en el vector
 * @return  True->Se ha pulsado
 */
bool PlayerController::IsKeyDown(int num){
	if(num < keys.size()){											// Comprueba que el numero no se salga del vector
		Key_player* k = keys[num];
		if(k->GetStatus()==DOWN || k->GetStatus()==PRESSED){		// Comprueba si el Status de la tecla es el correcto
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
bool PlayerController::IsKeyDown(ACTION_ENUM action){
	int size = keys.size();
	for(int i=0; i<size; i++){											// Recorremos todos el vector de teclas
		Key_player* k = keys[i];
		if(action == k->GetAction()){									// Miramos si se corresponde con la accion pasada
			if(k->GetStatus()==DOWN || k->GetStatus()==PRESSED){		// Comprobamos que el Status sea el correspondiente
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
bool PlayerController::IsKeyPressed(int num){
	if(num < keys.size()){
		Key_player* k = keys[num];
		if(k->GetStatus()==PRESSED) return true;
	}
	return false;
}

/**
 * Comprueba si una tecla esta siendo pulsada
 * 
 * @action - Nombre de la accion
 * @return  True->La tecla esta siendo pulsada
 */
bool PlayerController::IsKeyPressed(ACTION_ENUM action){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(action == k->GetAction()){
			if(k->GetStatus()==PRESSED) return true;
			return false;
		}
	}
	return false;
}

/**
 * Comprueba si una tecla esta siendo soltada
 * 
 * @num - Posicion de la tecla n el vector
 * @return  True->La tecla esta siendo pulsada
 */
bool PlayerController::IsKeyReleased(int num){
	if(num < keys.size()){
		Key_player* k = keys[num];
		if(k->GetStatus()==RELEASED) return true;
	}
	return false;
}

/**
 * Comprueba si una tecla esta siendo soltada
 * 
 * @action - Nombre de la accion
 * @return  True->La tecla esta siendo pulsada
 */
bool PlayerController::IsKeyReleased(ACTION_ENUM action){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(action == k->GetAction()){
			if(k->GetStatus()==RELEASED) return true;
			return false;
		}
	}
	return false;
}

/**
 * Comprueba si una tecla esta suelta
 * 
 * @num - Posicion de la tecla n el vector
 * @return  True->La tecla esta siendo pulsada
 */
bool PlayerController::IsKeyUp(int num){
	if(num < keys.size()){
		Key_player* k = keys[num];
		if(k->GetStatus()==UP) return true;
	}
	return false;
}

/**
 * Comprueba si una tecla esta suelta
 * 
 * @action - Nombre de la accion
 * @return  True->La tecla esta siendo pulsada
 */
bool PlayerController::IsKeyUp(ACTION_ENUM action){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(action == k->GetAction()){
			if(k->GetStatus()==UP) return true;
			return false;
		}
	}
	return false;
}

/**
 * Actualizacion de los Status de las teclas del vector
 */
void PlayerController::Update(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		keyStatesENUM status;
		status = g_engine->GetKeyStatus(k->GetKey());
		if(status == PRESSED || status == RELEASED) k->SetStatus(status);
	}
}

void PlayerController::SwapActions(ACTION_ENUM a, ACTION_ENUM b){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(a == k->GetAction()){
			k->SetAction(b);
		} else if( b == k->GetAction()){
			k->SetAction(a);
		}
	}
}

/**
 * Actualiza el status a DOWN o UP de las teclas
 */
void PlayerController::UpdateOwnStatus(){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		if(k->GetStatus() == PRESSED) k->SetStatus(DOWN);
		else if(k->GetStatus() == RELEASED) k->SetStatus(UP);
	}
}

void PlayerController::SetAllStatus(keyStatesENUM status){
	int size = keys.size();
	for(int i=0; i<size; i++){
		Key_player* k = keys[i];
		k->SetStatus(status);
	}
}