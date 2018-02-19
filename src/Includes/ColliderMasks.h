#ifndef COLLIDERMASKS_H
#define COLLIDERMASKS_H

#define BIT(x) (1<<(x))

enum CollisionTypes {
    C_NOTHING		= 0,	
    C_POTION		= BIT(0),
    C_GRAIL			= BIT(1),
    C_DOOR			= BIT(2),
	C_PLAYER		= BIT(3),
	C_FOUNTAIN		= BIT(4),
	C_SWITCH		= BIT(5),
	C_WALL			= BIT(6),
	C_PROJECTILE	= BIT(7),
	C_TRAP			= BIT(8),
	C_NPC			= BIT(9),
	C_CAMERA		= BIT(10),
	C_DAMAGEAREA	= BIT(12),
	C_BODYPART		= BIT(13)
};

static int doorCW			= C_PLAYER
							| C_PROJECTILE
							| C_POTION
							| C_BODYPART;

static int grailCW 			= C_PLAYER
							| C_PROJECTILE
							| C_POTION
							| C_BODYPART;

static int potionCW 		= C_DOOR
							| C_WALL
							| C_GRAIL
							| C_FOUNTAIN
							| C_NPC;

static int playerCW			= C_WALL
							| C_DOOR
							| C_GRAIL
							| C_FOUNTAIN
							| C_TRAP
							| C_PROJECTILE
							| C_PLAYER
							| C_NPC
							| C_DAMAGEAREA
							| C_BODYPART;

static int fountainCW		= C_WALL
							| C_DOOR
							| C_GRAIL
							| C_FOUNTAIN
							| C_PROJECTILE
							| C_PLAYER
							| C_BODYPART;

static int switchCW			= C_PROJECTILE
							| C_PLAYER
							| C_POTION;

static int wallCW			= C_PROJECTILE
							| C_PLAYER
							| C_POTION
							| C_CAMERA
							| C_BODYPART;

static int cameraCW			= C_WALL;

static int projectileCW		= C_WALL
							| C_PLAYER
							| C_GRAIL
							| C_SWITCH
							| C_TRAP
							| C_DOOR
							| C_FOUNTAIN
							| C_NPC;

static int trapCW			= C_PROJECTILE
							| C_PLAYER;

static int npcCW			= C_PROJECTILE
							| C_PLAYER
							| C_POTION
							| C_BODYPART;

static int raycastCW		= C_DOOR
							| C_GRAIL
							| C_POTION
							| C_FOUNTAIN
							| C_SWITCH
							| C_WALL
							| C_PROJECTILE
							| C_TRAP
							| C_NPC
							| C_PLAYER;

static int noPlayerRaycastCW		= C_DOOR
									| C_GRAIL
									| C_POTION
									| C_FOUNTAIN
									| C_SWITCH
									| C_WALL
									| C_PROJECTILE
									| C_TRAP
									| C_NPC;

static float damageareaCW		=	C_PLAYER;

static float bodypartCW		= C_DOOR
							| C_GRAIL
							| C_FOUNTAIN
							| C_WALL
							| C_NPC
							| C_PLAYER;

#endif