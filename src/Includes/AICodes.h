#ifndef AICODES_H
#define AICODES_H

enum AI_code
{
	AI_CHARACTER	= 0x00,	// Personaje de la IA
	AI_TARGET		= 0x01,	// Target de la IA (Kinematic Object)
	AI_SPELL_STATUS	= 0x02, // Estado de la habilidad que esta lanzando la IA
	//CODIGOS SENSE
	AI_PLAYER		= 0x02	// Codigo player sense
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01	// Oido
};

#endif