#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <string>
#include <vector>
#include <Menus.h>
#include <vector2d.h>
#include <Alliance.h>

class Player;
class Menu;
class GraphicEngine;
namespace irr{ namespace gui{ class IGUIWindow; } }

class MenuManager{
public:
	static MenuManager* GetInstance();
	~MenuManager();
	void Update(float deltaTime);
	void Draw();

	void CreateMenu(MenuType type, int option = -1);
	void ClearMenu();
	void ClearElement(MenuOption elementID);
	void UpdateNetDebug();
	bool CheckIfExists(MenuOption elementID);
	MenuType* GetCurrentMenu();

private:
	Menu* currentMenu;
	bool open_menu;

	static MenuManager*	instance; 
	std::vector<int>	loadedOptions;
	GraphicEngine*		g_engine;
	vector2di			screenSize;
	irr::gui::IGUIWindow* netDebugWindow;

	MenuManager();
	void CreateNetDebug();
	std::string GetStringFromPlayer(Player* player);

};

#endif