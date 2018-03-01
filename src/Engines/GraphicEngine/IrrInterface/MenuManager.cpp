#include "MenuManager.h"
#include "./../../Managers/SpellManager.h"
#include "./../../Managers/PlayerManager.h"
#include <TrapCodes.h>
#include <GUIEngine/GUIEngine.h>
#include "IrrEngine.h"
#include <MenusHeaders.h>

MenuManager* MenuManager::instance = nullptr;

MenuManager::MenuManager(){
	g_engine = IrrEngine::getInstance();
	irr::core::dimension2du res = g_engine->privateDriver->getScreenSize();
	screenSize.X = res.Width;
	screenSize.Y = res.Height;
	netDebugWindow = nullptr;
	open_menu = false;
	currentMenu = nullptr;
}

MenuManager::~MenuManager(){
	ClearMenu();
	instance = nullptr;
}

MenuManager* MenuManager::GetInstance(){
	if(instance == nullptr) instance = new MenuManager();
	return instance;
}

void MenuManager::CreateMenu(MenuType type, int option){
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
			if(netDebugWindow == nullptr) CreateNetDebug();
			else ClearMenu();
			break;
		}
		case(SELLER_M):{
			open_menu = true;
			currentMenu = new ShopMenu(type);
			//CreateSeller(option);
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
		g_engine->ToggleCameraMovement(false);
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
		currentMenu->Drop();
		currentMenu = nullptr;
	}
	netDebugWindow = nullptr;

}

MenuType* MenuManager::GetCurrentMenu(){
	MenuType *toRet = nullptr;
	if(currentMenu!=nullptr) toRet  = currentMenu->GetType();
	return toRet;
}
