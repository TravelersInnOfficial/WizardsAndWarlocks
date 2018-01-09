#ifndef AICODES_H
#define AICODES_H

enum AI_code
{
	AI_CHARACTER		= 0x00,	// Personaje de la IA
	AI_TARGET			= 0x01,	// (Puntero) Target de la IA (Kinematic Object)
	// SUB ARBOLES DE LAS TAREAS
	AI_TASK_DEFAULT 	= 0x02,	// (Puntero) SubArbol de decisiones de tareas basico
	AI_TASK_DRINK_POT	= 0x03, // (Puntero) SubArbol de decisiones de tarea beber pocion
	AI_TASK_CATCH_POT	= 0x04, // (Puntero) SubArbol de decisiones de tarea coger pocion
	AI_TASK_SHOOT_SPELL	= 0x05,	// (Puntero) SubArbol de decisiones de tarea disparar habilidad
	AI_TASK_SPELL00		= 0x06,	// (Puntero) SubArbol de decisiones del hechizo 00
	AI_TASK_SPELL01 	= 0x07,	// (Puntero) SubArbol de decisiones del hechizo 01
	AI_TASK_SPELL02 	= 0x08,	// (Puntero) SubArbol de decisiones del hechizo 02
	AI_TASK_SPELL03 	= 0x09,	// (Puntero) SubArbol de decisiones del hechizo 03
	// SUB ARBOLES DE MOVIMIENTO
	AI_MOVE_DEFAULT		= 0x0A,	// (Puntero) SubArbol de decisiones de movimiento basico
	AI_MOVE_SPELL00 	= 0x0B,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 00
	AI_MOVE_SPELL01 	= 0x0C,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 01
	AI_MOVE_SPELL02 	= 0x0D, // (Puntero) SubArbol de decisiones de movimiento del hechizo 02
	AI_MOVE_SPELL03 	= 0x0E, // (Puntero) SubArbol de decisiones de movimiento del hechizo 03
	//CODIGOS SENSE
	AI_PLAYER_WARL		= 0x0F,	// Codigo player sense
	AI_PLAYER_WIZA		= 0x10,	// Codigo player sense
	AI_POTION			= 0x11,	// Codigo pocion sense
	AI_FOUNTAIN			= 0x12,	// Codigo fuente sense
	AI_GRAIL			= 0x13, // Codigo grial sense
	AI_INVOCATION		= 0x14,	// Codigo invocacion sense
	AI_SWITCH			= 0x15,	// Codigo switch sense
	AI_DOOR				= 0x16,	// Codigo door sense
	AI_TRAP				= 0x17	// Codigo trap sense
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01	// Oido
};

static std::string AICode_str[] = {
	"CHARACTER",
	"TARGET",
	"TASK_DEFAULT", 
	"TASK_DRINK_POT", 
	"TASK_CATCH_POT", 
	"TASK_SHOOT_SPELL",
	"TASK_SPELL00", 
	"TASK_SPELL01", 
	"TASK_SPELL02", 
	"TASK_SPELL03", 
	"MOVE_DEFAULT", 
	"MOVE_SPELL00", 
	"MOVE_SPELL01", 
	"MOVE_SPELL02", 
	"MOVE_SPELL03", 
	"PLAYER_WARL", 
	"PLAYER_WIZA", 
	"POTION", 
	"FOUNTAIN", 
	"GRAIL", 
	"INVOCATION",
	"SWITCH", 
	"DOOR", 
	"TRAP"
};

#endif