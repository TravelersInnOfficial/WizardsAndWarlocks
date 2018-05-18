#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <iostream>
#include "./State.h"

class GraphicEngine;
class SoundEvent;
class GSprite;

class MenuPrincipal: public State{
public:
	MenuPrincipal();
	~MenuPrincipal();

	bool Input();
	void Update(float deltaTime);
	void Draw();

	void playMenuMusic();

private:
    std::string background;
	
	void PrepareServer();
	void PrepareClient(bool proprietary = false);
	void PrepareGame();

	void createSoundEvent();
	SoundEvent*		menuMusic;

	GraphicEngine* 	g_engine;			// Puntero al motor grafico del juego
	//MenuOption 		selectedOption;		// Opcion dada en el menu
	GSprite* bkg;
	int bkg_frame;
	int total_bkg_frames;
	
};


#endif