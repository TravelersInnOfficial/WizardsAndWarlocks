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
	void InitObject();
	void EmptyObject();

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
	float GetMinCostPM(Player* p);
	int GetNumSpells();
	std::vector<Hechizo*> GetSpells(Player* player);
	std::vector<std::string> GetSpellInfo(SPELLCODE spell);
	std::vector<float> GetSpellProps(SPELLCODE spell);

	std::string GetPathFromEnum(SPELLCODE sKind);

	SPELLCODE GetSpellCode(int num, Player* p);

	void ErasePlayer(Player* player);
	void RefreshServerAll();
	std::map<std::string, SPELLCODE> GetSPELLCODE_StrMap();

private:
	Hechizo* CrearHechizo(SPELLCODE type);
	
	//Por cada hechizo que pueda tener el jugador un hastable
	std::map<Player*, Hechizo*> hechizos[4];
	int numHechizos = 4;

	std::map<SPELLCODE, std::string> spell_names;
	std::map<SPELLCODE, std::string> spell_descriptions;
	std::map<SPELLCODE, std::string> spell_PROJECTILE;
	std::map<SPELLCODE, std::string> spell_EFFECT;
	
	std::map<SPELLCODE, float> spell_costMP;
	std::map<SPELLCODE, float> spell_tCast;
	std::map<SPELLCODE, float> spell_tCooldown;
	std::map<SPELLCODE, float> spell_optHP;
	std::map<SPELLCODE, float> spell_optMP;
	
	float m_deltaTime;

	void updateSoundEvents(Hechizo* h, Player* p); 	//Update sound event spell position, otherwise will sound far as we move
	void loadSpellsData();
	SpellManager();
	SpellManager(SpellManager&);
	SpellManager operator =(SpellManager&);
};

#endif