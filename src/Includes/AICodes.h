#ifndef AICODES_H
#define AICODES_H

enum AI_code
{
	AI_CHARACTER	= 0x00,	// Personaje de la IA
	AI_TARGET		= 0x01,	// (Puntero) Target de la IA (Kinematic Object)
	AI_SPELL_STATUS	= 0x02, // (Int) Estado de la habilidad que esta lanzando la IA
	AI_TASK_SPELL00	= 0x03,	// (Puntero) SubArbol de decisiones del hechizo 00
	AI_TASK_SPELL01 = 0x04,	// (Puntero) SubArbol de decisiones del hechizo 01
	AI_TASK_SPELL02 = 0x05,	// (Puntero) SubArbol de decisiones del hechizo 02
	AI_TASK_SPELL03 = 0x06,	// (Puntero) SubArbol de decisiones del hechizo 03
	//CODIGOS SENSE
	AI_PLAYER		= 0x07	// Codigo player sense
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01	// Oido
};

#endif