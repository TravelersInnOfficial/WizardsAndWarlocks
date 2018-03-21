#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <iostream>
#include <Menus.h>
#include <NetworkStructs.h>

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

	void playMenuMusic();

private:
    std::string background = "./../assets/textures/GUI/Menus/MainMenu/background.png";
	
	void PrepareServer();
	void PrepareClient(bool proprietary = false);
	void PrepareGame();

	void createSoundEvent();
	SoundEvent*		menuMusic;

	GraphicEngine* 	g_engine;			// Puntero al motor grafico del juego
	//MenuOption 		selectedOption;		// Opcion dada en el menu
	TFSprite* bkg;
	
};


#endif