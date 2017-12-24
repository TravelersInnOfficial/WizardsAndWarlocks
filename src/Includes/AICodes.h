#ifndef AICODES_H
#define AICODES_H

enum AI_code
{
	AI_CHARACTER	= 0x00,	//PERSONAJE DE LA IA
	AI_TARGET		= 0x01,	//TARGET DE LA IA
	//CODIGOS SENSE
	AI_PLAYER		= 0x02	//CODIGO PLAYER SENSE
};

enum AI_modalities
{
	AI_SIGHT		= 0x00,	// Vision
	AI_HEARING		= 0x01
};

#endif