#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <iostream>
#include <Menus.h>

#include "./../Managers/StateManager.h"
#include "./State.h"
#include "./../Menus/MainMenu.h"

class MenuPrincipal: public State{
public:
	MenuPrincipal();
	~MenuPrincipal();

	bool Input();
	void Update(float deltaTime);
	void Draw();
private:
    std::string background = "./../assets/textures/GUI/Menus/MainMenu/background.png";
	
	void PrepareServer();
	void PrepareClient();
	void PrepareGame();

	GraphicEngine* 	g_engine;			// Puntero al motor grafico del juego
	GUIEngine* 		GUI_engine;
	MenuOption 		selectedOption;		// Opcion dada en el menu
};


#endif