#include "Hechizo.h"
#include "./../ControlProyectil.h"
#include "./../GraphicEngine/GraphicEngine.h"

Hechizo::Hechizo(){
	costePM = 20;
	
	startTime = 0.0f;
	timeCasting = 0.0f;
	casting = 2.0f;

	currentCooldown = 0.0f;
	cooldown = 5.0f;
}

void Hechizo::EmpezarCast(){
	startTime = GraphicEngine::getInstance()->getTime() * 0.001;
	timeCasting = 0;
}

bool Hechizo::ComprobarCast(){
	if(currentCooldown<=0){							// Comprobamos que realmente se pueda lanzar el hechizo
		float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
		timeCasting += currentTime - startTime;		// Le sumamos la diferencia entre la ultima vez que se llamo al metodo
		startTime = currentTime;					// Nos guardamos cuando se llamo esta vez al metodo

		if(timeCasting>=casting){					// Comprobamos si el tiempo de casteo se ha completado
			timeCasting = 0.0f;						// Reseteamos el tiempo de casteo
			currentCooldown = cooldown;				// Ponemos el tiempo de cooldown
			return true;
		}
	}
	return false;
}

void Hechizo::Lanzar(Player* p){
	std::cout<<"Hey Lanzaste un hechizo normal"<<std::endl;
}

void Hechizo::DecCooldown(float time){
	currentCooldown -= time;
}

float Hechizo::GetCurrentCooldown(){
	return currentCooldown;
}