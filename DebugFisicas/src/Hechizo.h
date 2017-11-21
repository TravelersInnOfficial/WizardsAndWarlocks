#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>

using namespace std;

class Hechizo{
public:
	Hechizo();
	void EmpezarCast();
	void ComprobarCast();
	void Lanzar();
private:
	int 	costePM;
	
	float	startTime;
	float 	timeCasting;
	float 	casting;

	float 	currentCooldown;
	float	cooldown;
};

#endif