#include "Entidad.h"

Entidad::Entidad(){

}

Entidad::~Entidad(){
	
}

string Entidad::GetClase(){
	return clase;
}

void Entidad::Update(){}
void Entidad::Contact(void* punt, string tipo){}
void Entidad::Interact(){}