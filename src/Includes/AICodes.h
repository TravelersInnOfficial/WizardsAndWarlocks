#ifndef AICODES_H
#define AICODES_H

enum AI_code{
	//CODIGOS SENSE
	AI_PLAYER_WARL		= 0x00,	// Codigo player sense
	AI_PLAYER_WIZA		= 0x01,	// Codigo player sense
	AI_POTION			= 0x02,	// Codigo pocion sense
	AI_FOUNTAIN			= 0x03,	// Codigo fuente sense
	AI_GRAIL			= 0x04, // Codigo grial sense
	AI_INVOCATION		= 0x05,	// Codigo invocacion sense
	AI_SWITCH			= 0x06,	// Codigo switch sense
	AI_DOOR				= 0x07,	// Codigo door sense
	AI_TRAP				= 0x08,	// Codigo trap sense
	// SUB ARBOLES DE LAS TAREAS
	AI_TASK_DEFAULT 	= 0x09,	// (Puntero) SubArbol de decisiones de tareas basico
	AI_TASK_DEFUSE_TRAP	= 0x0A, // (Puntero) SubArbol de decisiones de desactivar trampa
	AI_TASK_DRINK_POT	= 0x0B, // (Puntero) SubArbol de decisiones de tarea beber pocion
	AI_TASK_CATCH_POT	= 0x0C, // (Puntero) SubArbol de decisiones de tarea coger pocion
	AI_TASK_USE_FOUNT	= 0x0D,	// (Puntero) SubArbol de decisiones de tarea usar fuente
	AI_TASK_ESCAPE		= 0x0E,	// (Puntero) SubArbol de decisiones de tarea escapar
	AI_TASK_SHOOT_SPELL	= 0x0F,	// (Puntero) SubArbol de decisiones de tarea disparar habilidad
	AI_TASK_SPELL00		= 0x10,	// (Puntero) SubArbol de decisiones del hechizo 00
	AI_TASK_SPELL01 	= 0x11,	// (Puntero) SubArbol de decisiones del hechizo 01
	AI_TASK_SPELL02 	= 0x12,	// (Puntero) SubArbol de decisiones del hechizo 02
	AI_TASK_SPELL03 	= 0x13,	// (Puntero) SubArbol de decisiones del hechizo 03
	// SUB ARBOLES DE MOVIMIENTO
	AI_MOVE_DEFAULT		= 0x14,	// (Puntero) SubArbol de decisiones de movimiento basico
	AI_MOVE_NO			= 0x15,	// (Puntero) SubArbol de decisiones sin movimiento
	AI_MOVE_GOTARGET	= 0x16, // (Puntero) SubArbol de decisiones de movimiento hacia el target
	AI_MOVE_SPELL00 	= 0x17,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 00
	AI_MOVE_SPELL01 	= 0x18,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 01
	AI_MOVE_SPELL02 	= 0x19, // (Puntero) SubArbol de decisiones de movimiento del hechizo 02
	AI_MOVE_SPELL03 	= 0x1A, // (Puntero) SubArbol de decisiones de movimiento del hechizo 03
	// VARIABLES DE BLACKBOARD
	AI_TARGET			= 0x1B,	// (Puntero) Target de la IA (Kinematic Object)
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01	// Oido
};

static std::string AICode_str[] = {
	"CHARACTER",				// 0
	"TARGET",					// 1
	"TASK_DEFAULT", 			// 2
	"AI_TASK_DEFUSE_TRAP",		// 3
	"TASK_DRINK_POT", 			// 4
	"TASK_CATCH_POT",			// 5
	"AI_TASK_USE_FOUNT",		// 6
	"TASK_SHOOT_SPELL",			// 7
	"TASK_SPELL00", 			// 8
	"TASK_SPELL01", 			// 9
	"TASK_SPELL02", 			// A
	"TASK_SPELL03", 			// B
	"MOVE_DEFAULT",				// C
	"AI_MOVE_NO",				// D
	"AI_MOVE_GOTARGET",
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