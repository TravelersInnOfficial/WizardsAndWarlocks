#include "Entidad.h"

Entidad::Entidad(){

}

Entidad::~Entidad(){
	
}

EntityEnum Entidad::GetClase(){
	return clase;
}

void Entidad::Update(){}
void Entidad::Contact(void* punt, EntityEnum tipo){}
void Entidad::Interact(){}