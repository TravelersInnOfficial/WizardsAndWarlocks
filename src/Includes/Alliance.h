#ifndef ALLIANCE_H
#define ALLIANCE_H

enum Alliance{
	ERR_ALLIANCE			= -1,
	// Igualacion hecha a mano para pasar de AI_code a la alianza
	ALLIANCE_WARLOCK		=  0,		// Brujos
	ALLIANCE_WIZARD			=  1,		// Magos
	NO_ALLIANCE				=  2  		// Sin Alianza (Situacion inicial)
};

#endif