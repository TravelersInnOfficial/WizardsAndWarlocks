#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include "./../Players/Player.h"

class Hechizo{
public:
	Hechizo(int costPM, float tCast, float tCoolDown);
	virtual ~Hechizo();

	void EmpezarCast();
	virtual void ResetSpell();
	bool ComprobarCast(float deltaTime);
	virtual void Lanzar(Player* p);
	float GetCurrentCooldown();
	void DecCooldown(float time);
	bool CheckMP(int MP);
	int	GetMP();
protected:
	int 	costePM;			// Coste de Mana del hechizo
	bool	lanzable;			// Se puede lanzar el hechizo? En Cooldown? Si/No

	float 	timeCasting;		// Tiempo que lleva casteando
	float 	casting;			// Tiempo total de casteo

	float 	currentCooldown;	// Tiempo actual de cooldown
	float	cooldown;			// Tiempo total de cooldown
};

#endif