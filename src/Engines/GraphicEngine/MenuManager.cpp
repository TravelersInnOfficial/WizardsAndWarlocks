#include "MenuManager.h"
#include "./../Players/Player.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include <TrapCodes.h>
#include <GUIEngine/GUIEngine.h>

MenuManager* MenuManager::instance = 0;

MenuManager::MenuManager(){
	g_engine = GraphicEngine::getInstance();
	irr::core::dimension2du res = g_engine->privateDriver->getScreenSize();
	screenSize.X = res.Width;
	screenSize.Y = res.Height;
	netDebugWindow = NULL;
	open_menu = false;
}

MenuManager::~MenuManager(){
	ClearMenu();
}

MenuManager* MenuManager::GetInstance(){
	if(instance == 0) instance = new MenuManager();
	return instance;
}

void MenuManager::CreateMenu(MenuType type, int option){
	switch(type){
		case(MAIN_M):{
			currentMenu = new MainMenu();
			open_menu = true;
			break;
		}
		case(OPTIONS_M):{
			currentMenu = new OptionsMenu();
			open_menu = true;
			break;
		}
		case(ALLIANCE_M):{
			currentMenu = new AllianceMenu();
			open_menu = true;
			break;
		}

		case(ENDMATCH_M):{
			currentMenu = new EndMatchMenu();
			open_menu = true;
			//CreateMatchEnded(option);
			break;
		}
		case(NETDEBUG_M):{
			if(netDebugWindow == NULL) CreateNetDebug();
			else ClearMenu();
			break;
		}
		case(SELLER_M):{
			currentMenu = new ShopMenu();
			open_menu = true;
			//CreateSeller(option);
			break;
		}
		default:{
			currentMenu = NULL;
			open_menu = false;
			break;
		}
	}
}

void MenuManager::Update(){
	//std::cout<<"OPEN VALUE? "<<open_menu<<std::endl;
	if(currentMenu != NULL && open_menu){ 
		GUIEngine::GetInstance()->Update();
		currentMenu->Update(&open_menu);
	}
}

void MenuManager::Draw(){
	if(currentMenu != NULL && open_menu){ 
		GUIEngine::GetInstance()->Draw();
	}
}

/******** OLD FUNCTIONS ********/
void MenuManager::ClearMenu(){
	int size = loadedOptions.size();
	for(int i=0; i<size; i++){
		irr::gui::IGUIElement* elem;
		elem = g_engine->privateGUIEnv->getRootGUIElement()->getElementFromId(loadedOptions[i], true);
		elem->remove();
	}
	loadedOptions.clear();
	netDebugWindow = NULL;
}

void MenuManager::ClearElement(MenuOption elementID){
	irr::gui::IGUIElement* elem;
	elem = g_engine->privateGUIEnv->getRootGUIElement()->getElementFromId(elementID, true);
	if(elem!= NULL) elem->remove();
}

void MenuManager::CreateNetDebug(){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X - 350, 0, screenSize.X, 200);

 	netDebugWindow = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Net Debug",
		0,
		NETDEBUG_M_WINDOW
	);
	netDebugWindow->setDraggable(false);
	netDebugWindow->getCloseButton()->setVisible(false);
	loadedOptions.push_back(NETDEBUG_M_WINDOW);

	UpdateNetDebug();
}

void MenuManager::UpdateNetDebug(){

	std::vector<Player*> players = PlayerManager::GetInstance()->GetAllPlayers();

	if(netDebugWindow != NULL){

		ClearElement(NETDEBUG_M_P1); ClearElement(NETDEBUG_M_P2); ClearElement(NETDEBUG_M_P3); ClearElement(NETDEBUG_M_P4); ClearElement(NETDEBUG_M_P5); ClearElement(NETDEBUG_M_P6); ClearElement(NETDEBUG_M_P7); ClearElement(NETDEBUG_M_P8);	
		vector2di menuSize = vector2di(netDebugWindow->getClientRect().getWidth(), netDebugWindow->getClientRect().getHeight());
		Player* player = NULL;

		std::vector<std::wstring> players_ws;
		std::wstring player1; std::wstring player2; std::wstring player3; std::wstring player4; std::wstring player5; std::wstring player6; std::wstring player7; std::wstring player8;
		players_ws.push_back(player1); players_ws.push_back(player2); players_ws.push_back(player3); players_ws.push_back(player4); players_ws.push_back(player5); players_ws.push_back(player6); players_ws.push_back(player7); players_ws.push_back(player8);

		std::vector<MenuOption> players_mo;
		players_mo.push_back(NETDEBUG_M_P1); players_mo.push_back(NETDEBUG_M_P2); players_mo.push_back(NETDEBUG_M_P3); players_mo.push_back(NETDEBUG_M_P4); players_mo.push_back(NETDEBUG_M_P5); players_mo.push_back(NETDEBUG_M_P6); players_mo.push_back(NETDEBUG_M_P7); players_mo.push_back(NETDEBUG_M_P8);

		for(int i = 0; i < 8; i++){
			if(players.size() > i){
				player = players.at(i);
				if(player != NULL){
					std::string ps = GetStringFromPlayer(player);
					players_ws.at(i) = std::wstring(ps.length(), L' ');
					std::copy(ps.begin(), ps.end(), players_ws.at(i).begin());
				}
			}
			else players_ws.at(i) = L"PLAYER NOT CONNECTED";
			vector4di rect = vector4di(0, menuSize.Y/8 * (i + 1), menuSize.X, menuSize.Y/8);
			g_engine->addStaticText(rect, players_ws.at(i), true, false, players_mo.at(i), netDebugWindow);
		}
	}
}

std::string MenuManager::GetStringFromPlayer(Player* player){
	std::vector<Hechizo*> spells = SpellManager::GetInstance()->GetSpells(player);
	std::string spells_s = "";
	for(int i = 1; i < spells.size(); i++){
		if(spells.at(i) != NULL){
			int spelltype = spells.at(i)->GetType();
			spells_s += std::to_string(spelltype) + " ";
		}
	}

	std::string toRet = player->GetName()
						+ "     |     "
						+ std::to_string((int)player->GetHP())
						+" HP / "
						+ std::to_string((int)player->GetMP())
						+" MP /"
						+ std::to_string((int)player->GetSP())
						+" SP.     |     Char: "
						+ std::to_string(player->GetHasCharacter())
						+ "     |     N_ID: "
						+ std::to_string(player->GetNetworkObject()->GetObjId())
						+ "     |     SPELLS: "
						+ spells_s;
	return(toRet);
}

bool MenuManager::CheckIfExists(MenuOption elementID){
	bool toRet = false;
	for(int i = 0; i < loadedOptions.size() && !toRet; i++){
		if(elementID == loadedOptions.at(i)) toRet = true;
	}
	return toRet;
}