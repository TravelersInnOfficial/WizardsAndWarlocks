#include "Entidad.h"

Entidad::Entidad(){

}

Entidad::~Entidad(){
	
}

std::string Entidad::GetClase(){
	return clase;
}

void Entidad::Update(){}
void Entidad::Contact(void* punt, std::string tipo){}