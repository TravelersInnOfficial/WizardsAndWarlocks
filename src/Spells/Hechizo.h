#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include "./../Players/Player.h"

class Hechizo{
public:
	Hechizo(int costPM, float tCast, float tCoolDown);
	virtual ~Hechizo();

	void EmpezarCast();
	bool ComprobarCast(float deltaTime);
	virtual void Lanzar(Player* p);
	float GetCurrentCooldown();
	void DecCooldown(float time);
	bool CheckMP(int MP);
	int	GetMP();
protected:
	int 	costePM;
	bool	lanzable;

	float 	timeCasting;
	float 	casting;

	float 	currentCooldown;
	float	cooldown;
};

#endif