#ifndef MENUS_H
#define MENUS_H

enum MenuType {
	MAIN_M		= 0,
	ALLIANCE_M	= 1,
	STORE_M		= 2,
	ENDMATCH_M	= 3
};

enum MenuOption {
	NO_OPT					= -1,

	MAIN_M_SINGLE			= 0,
	MAIN_M_TEXT_1			= 1,
	MAIN_M_CLIENT			= 2,
	MAIN_M_SERVER			= 3,
	MAIN_M_IP				= 4,
	MAIN_M_WINDOW			= 5,
	
	ALLIANCE_M_TEXT_1		= 6,
	ALLIANCE_M_WARLOCK 		= 7,
	ALLIANCE_M_WIZARD		= 8,
	ALLIANCE_M_WINDOW		= 9,
	ALLIANCE_M_CLOSE		= 10,

	ENDMATCH_M_TEXT			= 11,
	ENDMATCH_M_CONFIRM		= 12
};

#endif