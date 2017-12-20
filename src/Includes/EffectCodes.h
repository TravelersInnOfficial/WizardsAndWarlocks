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
	Gorro del Heraldo
	Reduce el daño producido por los proyectiles Elementales.
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
	WEAK_BASIC          	= 0x01,
	WEAK_BURNED				= 0x02,
	WEAK_FROZEN				= 0x03,
	WEAK_PARALYZED			= 0x04,
	WEAK_SLOWEDDOWN			= 0x05,
	WEAK_POISONED			= 0x06,
	WEAK_SILENCED			= 0x07,
	WEAK_GHOSTDISTORSION	= 0x08,
	WEAK_MADNESS			= 0x09,
	WEAK_DEATHSNARE			= 0x10,

	POWERUP_DAMAGE			= 0x11,
	POWERUP_DEFENSE			= 0x12,
	POWERUP_ELEMDEFENSE		= 0x13,
	POWERUP_SPEED			= 0x14,
	POWERUP_UNTARGET		= 0x15,
	POWERUP_FIRE			= 0x16,
	POWERUP_POISON			= 0x17,
	POWERUP_INVISIBLE		= 0x18
};

#endif