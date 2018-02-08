#ifndef STATE_CODES
#define STATE_CODES

enum State_Code{
	WITHOUT_STATE, 			// No hay estado
	STATE_MENU,				// Estado de menu de la partida
	STATE_GAME,				// Estado de partida normal
	STATE_NETGAME_CLIENT,	// Estado de partida en red Cliente
	STATE_NETGAME_SERVER	// Estado de partida en red Servidor
};

#endif