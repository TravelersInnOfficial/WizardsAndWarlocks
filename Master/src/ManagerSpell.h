#ifndef MANAGERSPELL_H
#define MANAGERSPELL_H

#include <iostream>
#include <map>
#include "./Spells/Hechizo.h"
#include "./Spells/HechizoProyectil.h"
#include "./Spells/SpellCodes.h"
#include "Player.h"

class ManagerSpell{
public:
	static ManagerSpell* GetInstance();
	~ManagerSpell();

	bool AddHechizo(int num, Player* p, SPELLCODE type);
	void UpdateCooldown();
	bool LanzarHechizo(int num, Player* p);
	void ResetHechizo(int num, Player* p);

private:
	Hechizo* CrearHechizo(SPELLCODE type);
	//Por cada hechizo que pueda tener el jugador un hastable
	std::map<Player*, Hechizo*> hechizos[4];
	int numHechizos = 4;

	ManagerSpell();
	static ManagerSpell* instance;
};

#endif