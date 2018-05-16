#include "MenuManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/PlayerManager.h"
#include <TrapCodes.h>
#include <GUIEngine/GUIEngine.h>
#include "GraphicEngine.h"
#include <MenusHeaders.h>

MenuManager* MenuManager::instance = nullptr;

MenuManager::MenuManager(){
	g_engine = GraphicEngine::getInstance();
	screenSize.X = g_engine->GetScreenWidth();
	screenSize.Y = g_engine->GetScreenHeight();
	//netDebugWindow = nullptr;
	open_menu = false;
	currentMenu = nullptr;
}

MenuManager::~MenuManager(){
	ClearMenu();
	instance = nullptr;
}

MenuManager* MenuManager::GetInstance(){
	static MenuManager instance = MenuManager();
	return &instance;
}

void MenuManager::CreateMenu(MenuType type, int option){
	if(currentMenu != nullptr) delete currentMenu;

	switch(type){
		case(MAIN_M):{
			open_menu = true;
			currentMenu = new MainMenu(type);
			break;
		}
		case(OPTIONS_M):{
			open_menu = true;
			currentMenu = new OptionsMenu(type);
			break;
		}
		case(ALLIANCE_M):{
			open_menu = true;
			currentMenu = new AllianceMenu(type);
			break;
		}

		case(ENDMATCH_M):{
			open_menu = true;
			currentMenu = new EndMatchMenu(type, option);
			break;
		}
		case(NETDEBUG_M):{
			/*if(netDebugWindow == nullptr) CreateNetDebug();
			else ClearMenu();
			*/
			break;
		}
		case(SELLER_M):{
			open_menu = true;
			currentMenu = new ShopMenu(type);
			//CreateSeller(option);
			break;
		}
		case(STATUS_M):{
			open_menu = true;
			currentMenu = new StatusMenu(type);
			//currentMenu = new AllianceMenu(type);
			break;
		}
		case(EXIT_MATCH_M):{
			open_menu = true;
			currentMenu = new ExitMatchMenu(type);
			break;
		}
		default:{
			currentMenu = nullptr;
			open_menu = false;
			break;
		}
	}
}

void MenuManager::Update(float deltaTime){
	GUIEngine::GetInstance()->Update();
	if(currentMenu != nullptr){
		currentMenu->Update(&open_menu, deltaTime);
	}
}

void MenuManager::Draw(){
	GUIEngine::GetInstance()->Draw();
	if(!open_menu) ClearMenu();
}

void MenuManager::ClearMenu(){
	if(currentMenu != nullptr ){
		if(open_menu) currentMenu->Close(&open_menu);
		//currentMenu->Drop();
		delete currentMenu;
		currentMenu = nullptr;
	}
	//netDebugWindow = nullptr;
}

MenuType* MenuManager::GetCurrentMenu(){
	MenuType *toRet = nullptr;
	if(currentMenu!=nullptr) toRet  = currentMenu->GetType();
	return toRet;
}
