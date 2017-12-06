#ifndef SPELLMANAGER_H
#define SPELLMANAGER_H

#include <iostream>
#include <map>
#include "./../Spells/Hechizo.h"
#include "./../Spells/HechizoProyectil.h"
#include <SpellCodes.h>
#include "./../Player.h"

class SpellManager{
public:
	static SpellManager* GetInstance();
	~SpellManager();

	bool AddHechizo(int num, Player* p, SPELLCODE type);
	void UpdateCooldown();
	bool LanzarHechizo(int num, Player* p);
	void ResetHechizo(int num, Player* p);

private:
	Hechizo* CrearHechizo(SPELLCODE type);
	//Por cada hechizo que pueda tener el jugador un hastable
	std::map<Player*, Hechizo*> hechizos[4];
	int numHechizos = 4;

	SpellManager();
	static SpellManager* instance;
};

#endif