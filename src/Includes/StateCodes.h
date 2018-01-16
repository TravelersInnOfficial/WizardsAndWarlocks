#ifndef STATE_CODES
#define STATE_CODES

enum State_Code{
	WITHOUT_STATE, 	// No hay estado
	STATE_MENU,		// Estado de menu de la partida
	STATE_GAME,		// Estado de partida normal
	STATE_NETGAME	// Estado de partida en red
};

#endif