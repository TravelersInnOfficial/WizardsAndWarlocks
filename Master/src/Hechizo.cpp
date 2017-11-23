#include "Hechizo.h"
#include "ControlProyectil.h"

Hechizo::Hechizo(){}

void Hechizo::EmpezarCast(){
	//startTime = irrlicht.getTime(); 
	timeCasting = 0;
}

void Hechizo::ComprobarCast(){
	//startTime = irrlicht.getTime
	//timeCasting += startTime; 
	if(timeCasting>=casting){
		currentCooldown = cooldown;
		Lanzar();
	}
}

void Hechizo::Lanzar(){
	//GraphicEngine* g = GraphicEngine::getInstance();
	//cout<<g->getTime()<<endl;
	
	//std::cout<<"Has lanzado el hechizo"<<std::endl;
	ControlProyectil* c = ControlProyectil::GetInstance();
	c->AddProyectil(1.0f, 0.0f, 0.0f, 0.2f, 5.0f);
}

void Hechizo::DecCooldown(float time){
	currentCooldown -= time;
}

float Hechizo::GetCurrentCooldown(){
	return currentCooldown;
}