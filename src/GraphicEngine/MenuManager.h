#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <iostream>
#include <vector>
#include <Menus.h>
#include <vector2d.h>
#include "GraphicEngine.h"

class MenuManager{
public:
	static MenuManager* GetInstance();
	~MenuManager();
	
	void CreateMenu(MenuType type);
	void ClearMenu();

private:

	static MenuManager*	instance; 
	std::vector<int>	loadedOptions;
	GraphicEngine*		g_engine;
	vector2di			screenSize;

	MenuManager();
	void CreateMain();
	void CreateAlliance();

};

#endif