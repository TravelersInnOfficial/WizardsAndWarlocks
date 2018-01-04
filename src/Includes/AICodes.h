#ifndef AICODES_H
#define AICODES_H

enum AI_code
{
	AI_CHARACTER	= 0x00,	// Personaje de la IA
	AI_TARGET		= 0x01,	// (Puntero) Target de la IA (Kinematic Object)
	// SUB ARBOLES DE LOS HECHIZOS
	AI_TASK_SPELL00	= 0x03,	// (Puntero) SubArbol de decisiones del hechizo 00
	AI_TASK_SPELL01 = 0x04,	// (Puntero) SubArbol de decisiones del hechizo 01
	AI_TASK_SPELL02 = 0x05,	// (Puntero) SubArbol de decisiones del hechizo 02
	AI_TASK_SPELL03 = 0x06,	// (Puntero) SubArbol de decisiones del hechizo 03
	// SUB ARBOLES DE MOVIMIENTO DE LOS HECHIZOS
	AI_MOVE_NOSPELL = 0x07,	// (Puntero) SubArbol de decisiones de movimiento sin hechizo
	AI_MOVE_SPELL00 = 0x08,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 00
	AI_MOVE_SPELL01 = 0x09,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 01
	AI_MOVE_SPELL02 = 0x0A, // (Puntero) SubArbol de decisiones de movimiento del hechizo 02
	AI_MOVE_SPELL03 = 0x0B, // (Puntero) SubArbol de decisiones de movimiento del hechizo 03
	//CODIGOS SENSE
	AI_PLAYER_WARL	= 0x0C,	// Codigo player sense
	AI_PLAYER_WIZA	= 0x0D,	// Codigo player sense
	AI_POTION		= 0x0E	// Codigo pocion sense
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01	// Oido
};

#endif