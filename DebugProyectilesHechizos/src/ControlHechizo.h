#ifndef CONTROLHECHIZO_H
#define CONTROLHECHIZO_H

#include <iostream>
#include <map>
#include "Hechizo.h"
#include "Player.h"

using namespace std;

//Clase controlador para tener guardadas las relaciones entre player y hechizos.
//Lo que no se es si para lanzar el hechizo debería pasar por esta clase

class ControlHechizo{
public:
	static ControlHechizo* GetInstance();

	bool AddHechizo(int num, Player* p);
	void UpdateCooldown();
	bool LanzarHechizo(int num, Player* p);

private:
	Hechizo* CrearHechizo();
	//Por cada hechizo que pueda tener el jugador un hastable
	std::map<Player*, Hechizo*> hechizos[4];
	int numHechizos = 4;

	ControlHechizo();
	static ControlHechizo* instance;
};

#endif