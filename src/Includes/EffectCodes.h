#ifndef EFFECTCODES_H
#define EFFECTCODES_H

/*
	MERMAS
	
	Quemadura
	Quita 5 HP/s por segundo durante 5 segundos y reduce la curación.
	Veneno
	Quita 10 HP/si el jugador ya tiene el efecto, en otro caso quita 5 HP/s durante 5 segundos.
	Congelado
	Reduce la velocidad de movimiento y cooldown al 100% durante 3 segundos.
	Ralentizado
	Reduce la velocidad de movimiento y cooldown al 50% durante 5 segundos.
	Parálisis
	Reduce la velocidad de movimiento y cooldown al 100% durante 3 segundos.
	Silenciar
	No te deja lanzar hechizos a excepción del ataque básico
	Distorsión Fantasmal
	Invoca un fantasma que distorsiona la vista del jugador durante su tiempo de actividad
	Locura
	Invierte los controles del usuario
	Inmovilización mortal
	Cancela el movimiento del usuario a la vez que le inflige 3 HP/s 


	POTENCIADORES

	Área de Tarion
	Aumenta el daño durante 7 segundos.
	Sombrero de protección Óhmica
	Reduce todo el daño recibido en un 50%.
	Las Chanclas de Hermes
	Aumenta la velocidad de movimiento, así como la de cooldown en un 15% durante 10 segundos	.
	Poncho Divino
	Te hace invulnerable a todo el daño durante un pequeño tiempo
	Bastón de Belcebú
	Tus ataques básicos aplican Quemadura
	Colmillo de Mandragora
	Tus ataques básicos aplican Veneno
	Invisibilidad
	Te hace invisible para los demás jugadores
	
*/

enum EFFECTCODE
{
	WEAK_BASIC = 0x01,
	WEAK_BURNED,
	WEAK_SOFTBURNED,
	WEAK_FROZEN,
	WEAK_PARALYZED,
	WEAK_SLOWEDDOWN,
	WEAK_POISONED,
	WEAK_SOFTPOISONED,
	WEAK_SILENCED,
	WEAK_GHOSTDISTORSION,
	WEAK_MADNESS,
	WEAK_DEATHSNARE,

	POWERUP_DAMAGE,
	POWERUP_DEFENSE,
	//POWERUP_ELEMDEFENSE,
	POWERUP_SPEED,
	POWERUP_UNTARGET,
	POWERUP_FIRE,
	POWERUP_POISON,
	POWERUP_INVISIBLE
};

#endif