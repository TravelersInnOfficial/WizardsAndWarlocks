#ifndef SPELLCODES_H
#define SPELLCODES_H

/*
Hechizos de Efecto
Rapidez Suprema (Táctico)
Aumenta la velocidad del jugador temporalmente

Ohmio Protección (Defensivo)
Hechizo que aplica sobre uno mismo el sombrero de protección Óhmica

Invocación de Tarion (Ofensivo)
Hechizo que aplica sobre uno mismo el área de Tarion

Capa de Invisibilidad (Defensivo)
Hace que el personaje se vuelva invisible durante un tiempo.

Poncho Divino (Defensivo)
Otorga protección contra todo tipo de hechizos durante un tiempo.

Hechizos de Proyectil
Ataque Básico (Ofensivo)
Hechizo que crea el proyectil base del juego, sin daño de ningún elemento.

Aliento del dragón (Ofensivo)
Hechizo que crea un proyectil de fuego que al impactar produce daño y quemaduras

Eructo de Ogro (Ofensivo)
Hechizo que crea un proyectil gaseoso que al impactar explota y llena un área de gas
venenoso.

Furia de Odín (Ofensivo)
Hechizo que crea un proyectil eléctrico que al impactar produce daño provoca parálisis.

Hechizos de Invocación
Muro de la Desesperación (Defensivo)

Clon Dummy (Defensivo)
Invoca un clon estático de tu personaje durante un tiempo

Base de Transporte (Táctico)
Dejas una pequeña base de teletransportación y al volver a ejecutar el hechizo te
teletransportas a la posición

Hechizos Instantáneos
Teleportación (Táctico)
Teleporta espacialmente a un jugador

Aguas de Gaia (Defensivo)
Hechizo que deshace todos los efectos secundarios que afecten al personaje

Soplo de Guiverno (Daño continuo)
Hechizo que crea un área en la que el enemigo recibe daño, es ralentizado. En el caso de
que se le aplique el efecto continuamente será congelado
*/

enum SPELLCODE{
	NO_SPELL = -1,

	// Effect Spells
	SPELL_SPEED,
	SPELL_DEFENSE,
	SPELL_INVISIBILITY,
	SPELL_UNTARGET,

	// Projectil Spells
	SPELL_BASIC,				// default code for spell
	SPELL_PROJECTILE,			// default spell - not changable on store
	SPELL_FIRE,
	SPELL_POISON,
	SPELL_THUNDER,

	// Special Spells
	SPELL_TELEPORT,
	SPELL_CLEANSE,

	// Invocation Spells
	SPELL_WALL,
	SPELL_DUMMY,
	SPELL_TELEPORTBASE,

	// Continuous Spells
	SPELL_BLIZZARD
};

#endif