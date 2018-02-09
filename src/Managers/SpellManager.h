#ifndef SPELLMANAGER_H
#define SPELLMANAGER_H

#include <iostream>
#include <map>
#include <SpellCodes.h>
#include "../Spells/Hechizo.h"
#include "./../Players/Player.h"

class SpellManager{
public:
	static SpellManager* GetInstance();
	~SpellManager();

	bool AddHechizo(int num, Player* p, SPELLCODE type, bool broadcast = true);
	
	void UpdateCooldown(float deltaTime);
	void ResetCooldown(Player* p);
	
	bool LanzarHechizo(int num, Player* p);
	bool StartHechizo(int num, Player* p);
	void ResetHechizo(int num, Player* p);
	void ResetAllDieHechizo();
	void ResetDieHechizo(Player* p);
	void ResetHechizo(Player* p);
	
	float GetUtility(int num, Player* p);
	int GetNumSpells();
	std::vector<Hechizo*> GetSpells(Player* player);

	void DrawHUDSpells(Player* p, int current);
	std::string GetPathFromEnum(SPELLCODE sKind);

	SPELLCODE GetSpellCode(int num, Player* p);

	void ErasePlayer(Player* player);
	void RefreshServerAll();

private:
	Hechizo* CrearHechizo(SPELLCODE type);
	
	//Por cada hechizo que pueda tener el jugador un hastable
	std::map<Player*, Hechizo*> hechizos[4];
	int numHechizos = 4;
	
	float m_deltaTime;

	SpellManager();
	void updateSoundEvents(Hechizo* h, Player* p); 	//Update sound event spell position, otherwise will sound far as we move
	static SpellManager* instance;

};

#endif