#ifndef IRRMENUMANAGER_H
#define IRRMENUMANAGER_H

#include <string>
#include <vector>
#include <Menus.h>
#include <vector2d.h>
#include <Alliance.h>

class Player;
class Menu;
class IrrEngine;
namespace irr{ namespace gui{ class IGUIWindow; } }

class IrrMenuManager{
public:
	static IrrMenuManager* GetInstance();
	~IrrMenuManager();
	void Update(float deltaTime);
	void Draw();

	void CreateMenu(MenuType type, int option = -1);
	void ClearMenu();
	MenuType* GetCurrentMenu();

private:
	Menu* currentMenu;
	bool open_menu;

	static IrrMenuManager*	instance; 
	std::vector<int>	loadedOptions;
	IrrEngine*		g_engine;
	vector2di			screenSize;
	irr::gui::IGUIWindow* netDebugWindow;

	IrrMenuManager();

};

#endif