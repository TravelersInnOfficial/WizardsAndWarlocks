#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include "./../Players/Player.h"
#include "./../SoundEngine/SoundSystem.h"
#include <SpellCodes.h>

class Hechizo{
public:
	Hechizo(float costPM, float tCast, float tCoolDown, SPELLCODE code, std::string HUDMiniature, float optHP, float optMP);
	virtual ~Hechizo();

	void EmpezarCast();
	void ResetCooldown();
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
	SoundEvent* getShotEvent();

	std::string GetHUDTexturePath();

protected:
	float 	costePM;			// Coste de Mana del hechizo
	bool	lanzable;			// Se puede lanzar el hechizo? En Cooldown? Si/No

	float 	timeCasting;		// Tiempo que lleva casteando
	float 	casting;			// Tiempo total de casteo

	float 	currentCooldown;	// Tiempo actual de cooldown
	float	cooldown;			// Tiempo total de cooldown

	float 	optimHP;			// Coste optimo de vida para lanzar (IA)
	float 	optimMP;			// Coste optimo de mana para lanzar (IA)
	std::string HUDTexturePath; // Textura de la miniatura en el hud 

	SoundEvent* shotEvent;		//Shot sound event

	SPELLCODE type;

	//Sound Functions
	virtual void createSoundEvent();
	void playShotEvent(vector3df pos);
};

#endif