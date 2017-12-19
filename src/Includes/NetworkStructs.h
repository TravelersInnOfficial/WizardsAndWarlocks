#ifndef NETWORKSTRUCTS_H
#define NETWORKSTRUCTS_H

#include "./../NetworkEngine/RakNetIncludes.h"

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
	ID_NO_OBJ 			= -1,
	ID_PLAYER_O			= 1,
	ID_POTION_O			= 2,
	ID_FOUNTAIN_O 		= 3,
	ID_DOOR_O 			= 4,
	ID_SWITCH_O 		= 5,
	ID_GRAIL_O 			= 6,
	ID_TRAP_O			= 7,
	ID_PROYECTIL_O		= 8
};

// 0 == NULL
enum ObjectVariable{
	ID_NO_VAR = -1,

	// ######################### PLAYER #########################
	PLAYER_MOVE_UP			=	1,
	PLAYER_MOVE_DOWN		=	2,
	PLAYER_MOVE_LEFT		=	3,
	PLAYER_MOVE_RIGHT		=	4,
	PLAYER_RAYCAST			=	5,
	PLAYER_JUMP				=	6,
	PLAYER_USE_OBJECT		=	7,
	PLAYER_SHOOT			=	8,
	PLAYER_POSITION			=	9,
	PLAYER_ROTATION			=	10,
	PLAYER_DEPLOY_TRAP 		=	11,
	PLAYER_ALLIANCE			=	12,
	PLAYER_RESET_RECEIVER	=	13
	
};


#endif