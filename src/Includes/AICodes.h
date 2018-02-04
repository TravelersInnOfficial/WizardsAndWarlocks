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
	AI_TASK_EXPLORE 	= 0x0A,	// (Puntero) SubArbol de decisiones de explorar sala
	AI_TASK_DEFUSE_TRAP	= 0x0B, // (Puntero) SubArbol de decisiones de desactivar trampa
	AI_TASK_DRINK_POT	= 0x0C, // (Puntero) SubArbol de decisiones de tarea beber pocion
	AI_TASK_CATCH_POT	= 0x0D, // (Puntero) SubArbol de decisiones de tarea coger pocion
	AI_TASK_USE_FOUNT	= 0x0E,	// (Puntero) SubArbol de decisiones de tarea usar fuente
	AI_TASK_ESCAPE		= 0x0F,	// (Puntero) SubArbol de decisiones de tarea escapar
	AI_TASK_SHOOT_SPELL	= 0x10,	// (Puntero) SubArbol de decisiones de tarea disparar habilidad
	AI_TASK_SPELL00		= 0x11,	// (Puntero) SubArbol de decisiones del hechizo 00
	AI_TASK_SPELL01 	= 0x12,	// (Puntero) SubArbol de decisiones del hechizo 01
	AI_TASK_SPELL02 	= 0x13,	// (Puntero) SubArbol de decisiones del hechizo 02
	AI_TASK_SPELL03 	= 0x14,	// (Puntero) SubArbol de decisiones del hechizo 03
	// SUB ARBOLES DE MOVIMIENTO
	AI_MOVE_DEFAULT		= 0x15,	// (Puntero) SubArbol de decisiones de movimiento basico
	AI_MOVE_NO			= 0x16,	// (Puntero) SubArbol de decisiones sin movimiento
	AI_MOVE_GOTARGET	= 0x17, // (Puntero) SubArbol de decisiones de movimiento hacia el target
	AI_MOVE_SPELL00 	= 0x18,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 00
	AI_MOVE_SPELL01 	= 0x19,	// (Puntero) SubArbol de decisiones de movimiento del hechizo 01
	AI_MOVE_SPELL02 	= 0x1A, // (Puntero) SubArbol de decisiones de movimiento del hechizo 02
	AI_MOVE_SPELL03 	= 0x1B, // (Puntero) SubArbol de decisiones de movimiento del hechizo 03
	// VARIABLES DE BLACKBOARD
	AI_TARGET			= 0x1C,	// (Puntero) Target de la IA (Kinematic Object)
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01	// Oido
};

static std::string AICode_str[] = {
	"AI_PLAYER_WARL",
	"AI_PLAYER_WIZA",
	"AI_POTION",
	"AI_FOUNTAIN",
	"AI_GRAIL",
	"AI_INVOCATION",
	"AI_SWITCH",
	"AI_DOOR",
	"AI_TRAP",
	"AI_TASK_DEFAULT",
	"AI_TASK_EXPLORE",
	"AI_TASK_DEFUSE_TRAP",
	"AI_TASK_DRINK_POT",
	"AI_TASK_CATCH_POT",
	"AI_TASK_USE_FOUNT",
	"AI_TASK_ESCAPE",
	"AI_TASK_SHOOT_SPELL",
	"AI_TASK_SPELL00",
	"AI_TASK_SPELL01",
	"AI_TASK_SPELL02",
	"AI_TASK_SPELL03",
	"AI_MOVE_DEFAULT",
	"AI_MOVE_NO",
	"AI_MOVE_GOTARGET",
	"AI_MOVE_SPELL00",
	"AI_MOVE_SPELL01",
	"AI_MOVE_SPELL02",
	"AI_MOVE_SPELL03",
	"AI_TARGET",
};

#endif