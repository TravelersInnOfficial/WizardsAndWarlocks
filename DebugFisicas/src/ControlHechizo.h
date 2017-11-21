#ifndef CONTROLHECHIZO_H
#define CONTROLHECHIZO_H

#include <iostream>
#include <map>
#include "Hechizo.h"
#include "Player.h"


class ControlHechizo{
public:
	ControlHechizo();
	Hechizo* AddHechizo(Player* p);

private:
	map<Hechizo*,Player*> relaciones;

};

#endif