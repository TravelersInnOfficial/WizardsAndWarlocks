#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <iostream>
#include <Menus.h>

#include "./../Managers/StateManager.h"
#include "./State.h"

#include <NetworkEngine/NetSeeker.h>

class MenuPrincipal: public State{
public:
	MenuPrincipal();
	~MenuPrincipal();

	bool Input();
	void Update(float deltaTime);
	void Draw();

private:
	void PrepareServer();
	void PrepareClient(bool proprietary = false);
	void PrepareGame();
	void UpdateSeeker(float deltaTime);

	GraphicEngine* 	g_engine;			// Puntero al motor grafico del juego
	MenuOption 		selectedOption;		// Opcion dada en el menu
	
	NetSeeker*		netSeeker;
	std::vector<std::string> lobbyList;
	
};


#endif