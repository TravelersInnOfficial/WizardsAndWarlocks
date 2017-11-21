#include "ControlHechizo.h"

ControlHechizo::ControlHechizo(){}

Hechizo* ControlHechizo::AddHechizo(Player* p){
	Hechizo* h = new Hechizo();
	relaciones[h] = p;
	return h;
}