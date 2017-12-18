#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <iostream>
#include <vector>
#include <Menus.h>
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

	MenuManager();
	void CreateMain();

};

#endif