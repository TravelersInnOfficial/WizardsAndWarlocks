#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include "./../Players/Player.h"
#include <SpellCodes.h>
#include <math.h>

class Hechizo{
public:
	Hechizo(float costPM, float tCast, float tCoolDown, SPELLCODE code, float optHP, float optMP);
	virtual ~Hechizo();

	void EmpezarCast();
	virtual void ResetSpell();
	bool ComprobarCast(float deltaTime);
	virtual void Lanzar(Player* p);
	float GetCurrentCooldown();
	void DecCooldown(float time);
	bool CheckMP(float MP);
	// Utility
	float GetUtility(Player* p);
	float CalculateUtility(float value, float base);
	// Getters
	float	GetMP();
	SPELLCODE GetType();

protected:
	float 	costePM;			// Coste de Mana del hechizo
	bool	lanzable;			// Se puede lanzar el hechizo? En Cooldown? Si/No

	float 	timeCasting;		// Tiempo que lleva casteando
	float 	casting;			// Tiempo total de casteo

	float 	currentCooldown;	// Tiempo actual de cooldown
	float	cooldown;			// Tiempo total de cooldown

	float 	optimHP;			// Coste optimo de vida para lanzar (IA)
	float 	optimMP;			// Coste optimo de mana para lanzar (IA)

	SPELLCODE type;
};

#endif