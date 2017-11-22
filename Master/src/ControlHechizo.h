#ifndef CONTROLHECHIZO_H
#define CONTROLHECHIZO_H

#include <iostream>
#include <map>
#include "Hechizo.h"
#include "Player.h"

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