#ifndef SPELLMANAGER_H
#define SPELLMANAGER_H

#include <iostream>
#include <map>
#include "./../Spells/AllSpells.h"
#include <SpellCodes.h>
#include "./../Players/Player.h"

class SpellManager{
public:
	static SpellManager* GetInstance();
	~SpellManager();

	bool AddHechizo(int num, Player* p, SPELLCODE type);
	void UpdateCooldown(float deltaTime);
	void ResetCooldown(Player* p);
	bool LanzarHechizo(int num, Player* p);
	bool StartHechizo(int num, Player* p);
	void ResetHechizo(int num, Player* p);
	float GetUtility(int num, Player* p);

private:
	Hechizo* CrearHechizo(SPELLCODE type);
	//Por cada hechizo que pueda tener el jugador un hastable
	std::map<Player*, Hechizo*> hechizos[4];
	int numHechizos = 4;
	float deltaTime;

	SpellManager();
	void updateSoundEvents(Hechizo* h, Player* p); 	//Update sound event spell position, otherwise will sound far as we move
	static SpellManager* instance;
};

#endif