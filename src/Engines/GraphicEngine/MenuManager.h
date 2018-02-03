#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <iostream>
#include <vector>
#include <Menus.h>
#include <vector2d.h>
#include "GraphicEngine.h"

#include <Alliance.h>
#include <MenusHeaders.h>

class Player;

class MenuManager{
public:
	static MenuManager* GetInstance();
	~MenuManager();
	void Update();

	void CreateMenu(MenuType type, int option = -1);
	void ClearMenu();
	void ClearElement(MenuOption elementID);
	void UpdateNetDebug();

private:
	Menu* currentMenu;

	static MenuManager*	instance; 
	std::vector<int>	loadedOptions;
	GraphicEngine*		g_engine;
	vector2di			screenSize;
	irr::gui::IGUIWindow* netDebugWindow;

	MenuManager();
	void CreateAlliance();
	void CreateMatchEnded(int option = -1);
	void CreateNetDebug();
	void CreateSeller(int option = -1);

	std::string GetStringFromPlayer(Player* player);

};

#endif