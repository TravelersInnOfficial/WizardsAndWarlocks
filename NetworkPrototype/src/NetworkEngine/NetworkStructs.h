#ifndef NETWORKSTRUCTS_H
#define NETWORKSTRUCTS_H

#include "RakNetIncludes.h"

enum GameMessages{
	ID_PLAYER_JOIN			= ID_USER_PACKET_ENUM + 1,
	ID_EXISTING_PLAYER		= ID_USER_PACKET_ENUM + 2,
	ID_PLAYER_DISCONNECT	= ID_USER_PACKET_ENUM + 3,
	ID_OBJECT_STATUS_CHAGED	= ID_USER_PACKET_ENUM + 4
};

enum VariableMapID{
	ID_CREATE			= 1,
	ID_EXISTING_OBJECT	= 2,
	ID_REMOVE			= 3,
	
	ID_CHANGE_BOOL		= 4,
	ID_CHANGE_INT		= 5,
	ID_CHANGE_FLOAT		= 6,
	ID_CHANGE_VECINT	= 7,
	ID_CHANGE_VECFLOAT	= 8
};

// ####################### GAME SPECIFIC MESSAGES #######################

enum ObjectType{
	ID_PLAYER_O			= 1,
	ID_POTION_O			= 2,
	ID_FOUNTAINS_O 		= 3,
	ID_DOOR_O 			= 4,
	ID_SWITCH_O 		= 5,
	ID_GRAIL_O 			= 6,
	ID_TRAP_O			= 7,
	ID_PROYECTIL_O		= 8
};

enum ObjectVariable{
	ID_NO_VAR = -1,

	// ######################### PLAYER #########################
	//BOOL
	PLAYER_ACTIVATED = 1,
	//INT
	//FLOAT
	//VI
	//VF
	PLAYER_POSITION = 1

	// ######################### POTION #########################
	//BOOL
	//INT
	//FLOAT
	//VI
	//VF
	
	// ######################### PROYECTIL #########################
	//BOOL
	//INT
	//FLOAT
	//VI
	//VF
	
};


#endif