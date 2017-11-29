#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include "./../Player.h"

class Hechizo{
public:
	Hechizo(int costPM, float tCast, float tCoolDown);
	void EmpezarCast();
	bool ComprobarCast();
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