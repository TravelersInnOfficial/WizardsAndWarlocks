#ifndef MENUS_H
#define MENUS_H

enum MenuType {
	MAIN_M		= 0,
	ALLIANCE_M	= 1,
	STORE_M		= 2,
	ENDMATCH_M	= 3,
	NETDEBUG_M	= 4,
	SELLER_M	= 5
};

enum MenuOption {
	NO_OPT					= -1,

// ####################################

	MAIN_M_SINGLE			= 0,
	MAIN_M_TEXT_1			= 1,
	MAIN_M_CLIENT			= 2,
	MAIN_M_SERVER			= 3,
	MAIN_M_IP				= 4,
	MAIN_M_NAME				= 5,
	MAIN_M_WINDOW			= 6,

// ####################################

	ALLIANCE_M_TEXT_1		= 7,
	ALLIANCE_M_WARLOCK 		= 8,
	ALLIANCE_M_WIZARD		= 9,
	ALLIANCE_M_WINDOW		= 10,

// ####################################

	ENDMATCH_M_TEXT_1		= 11,
	ENDMATCH_M_CONFIRM		= 12,
	ENDMATCH_M_WINDOW		= 13,

// ####################################

	NETDEBUG_M_P1			= 14,
	NETDEBUG_M_P2			= 15,
	NETDEBUG_M_P3			= 16,
	NETDEBUG_M_P4			= 17,
	NETDEBUG_M_P5			= 18,
	NETDEBUG_M_P6			= 19,
	NETDEBUG_M_P7			= 20,
	NETDEBUG_M_P8			= 21,
	NETDEBUG_M_WINDOW		= 22,

// ####################################

	SELLER_M_WINDOW			= 23,
	SELLER_M_TEXT_1			= 24,
	SELLER_M_PS_2			= 25,
	SELLER_M_PS_3			= 26,
	SELLER_M_PS_4			= 27,
	SELLER_M_TRAP			= 28,
	SELLER_M_ACCEPT			= 29,

	SELLER_DEATH_CLAWS		= 30,
    SELLER_SPIRITS			= 31,
    SELLER_SILENCE			= 32,
    SELLER_TAXES			= 33,
    SELLER_DISTURBANCE		= 34,
    SELLER_EXPLOSIVE		= 35,

	SELLER_SPEED			= 36,
	SELLER_DEFENSE			= 37,
	SELLER_INVISIBILITY		= 38,
	SELLER_UNTARGET			= 39,
	SELLER_FIRE				= 40,
	SELLER_POISON			= 41,
	SELLER_THUNDER			= 42,
	SELLER_TELEPORT			= 43,
	SELLER_CLEANSE			= 44,
	SELLER_WALL				= 45,
	SELLER_DUMMY			= 46,
	SELLER_TELEPORTBASE		= 47,
	SELLER_BLIZZAR			= 48


};

#endif