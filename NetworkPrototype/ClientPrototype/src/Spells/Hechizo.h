#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include "./../Player.h"

class Hechizo{
public:
	Hechizo();
	void EmpezarCast();
	bool ComprobarCast();
	virtual void Lanzar(Player* p);
	float GetCurrentCooldown();
	void DecCooldown(float time);
protected:
	int 	costePM;
	
	float	startTime;
	float 	timeCasting;
	float 	casting;

	float 	currentCooldown;
	float	cooldown;
};

#endif