#include "MenuManager.h"
#include "./../Players/Player.h"

MenuManager* MenuManager::instance = 0;

MenuManager::MenuManager(){
	g_engine = GraphicEngine::getInstance();
	irr::core::dimension2du res = g_engine->privateDriver->getScreenSize();
	screenSize.X = res.Width;
	screenSize.Y = res.Height;
	netDebugWindow = NULL;
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
			CreateMain();
			break;
		}
		case(ALLIANCE_M):{
			CreateAlliance();
			break;
		}
		case(ENDMATCH_M):{
			CreateMatchEnded(option);
			break;
		}
		case(NETDEBUG_M):{
			if(netDebugWindow == NULL) CreateNetDebug();
			else ClearMenu();
			break;
		}
		default:{
			break;
		}
	}
}

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

void MenuManager::CreateMain(){

	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Start your Game",
		0,
		MAIN_M_WINDOW
	);
	window->getCloseButton()->setVisible(false);
	window->setDraggable(false);
	loadedOptions.push_back(MAIN_M_WINDOW);

	vector4di rect = vector4di(menuSize.X/10.0f,menuSize.Y/3,menuSize.X/4.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Single Player", L"To play single player", MAIN_M_SINGLE, window);

	rect = vector4di(menuSize.X/1.59f,menuSize.Y/3.75f,menuSize.X/9,menuSize.Y/30);
	g_engine->addStaticText(rect, L"Multi Player", true, false, MAIN_M_TEXT_1, window);

	rect = vector4di(menuSize.X/2.3f,menuSize.Y/3,menuSize.X/4.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Conect to Game", L"To start a client (FILL IP)", MAIN_M_CLIENT, window);

	rect = vector4di(menuSize.X/1.4f,menuSize.Y/3,menuSize.X/4.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Host a Game", L"To start a server on this computer", MAIN_M_SERVER, window);

	rect = vector4di(menuSize.X/2.3f,menuSize.Y/1.4f,menuSize.X/4.5f,menuSize.Y/30);
	g_engine->addEditBox(rect, L"127.0.0.1", MAIN_M_IP, window);

	rect = vector4di(menuSize.X/2.3f,menuSize.Y/1.3f,menuSize.X/4.5f,menuSize.Y/30);
	g_engine->addEditBox(rect, L"Player Name", MAIN_M_NAME, window);
}

void MenuManager::CreateAlliance(){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Select Alliance",
		0,
		ALLIANCE_M_WINDOW
	);
	window->setDraggable(false);
	window->getCloseButton()->setVisible(false);
	loadedOptions.push_back(ALLIANCE_M_WINDOW);

	vector4di rect = vector4di(menuSize.X/2-((menuSize.X/7.0f)/2),menuSize.Y/4,menuSize.X/7.0f,menuSize.Y/30);
	g_engine->addStaticText(rect, L"Select your Alliance, Human.", true, false, ALLIANCE_M_TEXT_1, window);
	
	rect = vector4di(menuSize.X/9.0f,menuSize.Y/3,menuSize.X/3.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Play as a Wizard", L"Be a Wizard and get back the Grail", ALLIANCE_M_WIZARD, window);

	rect = vector4di(menuSize.X/1.67f,menuSize.Y/3,menuSize.X/3.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Play as a Warlock", L"Be a Warlock and protect the Grail", ALLIANCE_M_WARLOCK, window);
}

void MenuManager::CreateMatchEnded(int option){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30);
	vector2di menuSize = vector2di(menuWindow.getWidth(), menuWindow.getHeight());

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        menuWindow,
        false,
        L"Match Ended",
		0,
		ENDMATCH_M_WINDOW
	);
	window->setDraggable(false);
	window->getCloseButton()->setVisible(false);
	loadedOptions.push_back(ENDMATCH_M_WINDOW);

	std::wstring winner = L"THE WIZARDS WON!";
	if (option != (int)ALLIANCE_WIZARD) winner = L"THE WARLOCKS WON!";

	vector4di rect = vector4di(menuSize.X/2-((menuSize.X/9.0f)/2),menuSize.Y/4,menuSize.X/9.0f,menuSize.Y/30);
	g_engine->addStaticText(rect, winner, true, false, ENDMATCH_M_TEXT_1, window);

	rect = vector4di(menuSize.X/2-((menuSize.X/3.0f)/2),menuSize.Y/3,menuSize.X/3.0f,menuSize.Y/3);
	g_engine->addButton(rect, L"Accept", L"Go Back to the Lobby", ENDMATCH_M_CONFIRM, window);
}

void MenuManager::CreateNetDebug(){
	irr::core::rect<irr::s32> menuWindow = irr::core::rect<irr::s32>(screenSize.X - 200, 0, screenSize.X, 200);

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

	UpdateNetDebug(std::vector<Player*>());
}

void MenuManager::UpdateNetDebug(std::vector<Player*> players){

	if(netDebugWindow != NULL){

		ClearElement(NETDEBUG_M_P1); ClearElement(NETDEBUG_M_P2); ClearElement(NETDEBUG_M_P3); ClearElement(NETDEBUG_M_P4); ClearElement(NETDEBUG_M_P5); ClearElement(NETDEBUG_M_P6); ClearElement(NETDEBUG_M_P7); ClearElement(NETDEBUG_M_P8);	
		std::wstring player1; std::wstring player2; std::wstring player3; std::wstring player4; std::wstring player5; std::wstring player6; std::wstring player7; std::wstring player8;
		vector2di menuSize = vector2di(netDebugWindow->getClientRect().getWidth(), netDebugWindow->getClientRect().getHeight());

		// ##################################################

		if(players.size() > 0 && players.at(0) != NULL){
			std::string player1_s = players.at(0)->GetName() + ": " + std::to_string((int)players.at(0)->GetHP()) +" HP / "+ std::to_string((int)players.at(0)->GetMP()) +" MP";
			player1 = std::wstring(player1_s.length(), L' ');
			std::copy(player1_s.begin(), player1_s.end(), player1.begin());
		}
		else player1 = L"PLAYER 1 NOT CONNECTED";

		vector4di rect = vector4di(0, menuSize.Y/8, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player1, true, false, NETDEBUG_M_P1, netDebugWindow);
		
		// ##################################################

		if(players.size() > 1 && players.at(1) != NULL){
			std::string player2_s = players.at(1)->GetName() + ": " + std::to_string((int)players.at(1)->GetHP()) +" HP / "+ std::to_string((int)players.at(1)->GetMP()) +" MP";
			player2 = std::wstring(player2_s.length(), L' ');
			std::copy(player2_s.begin(), player2_s.end(), player2.begin());
		}
		else player2 = L"PLAYER 2 NOT CONNECTED";
		
		rect = vector4di(0, menuSize.Y/8*2, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player2, true, false, NETDEBUG_M_P2, netDebugWindow);
		
		// ##################################################
		
		if(players.size() > 2 && players.at(2) != NULL){
			std::string player3_s = players.at(2)->GetName() + ": " + std::to_string((int)players.at(2)->GetHP()) +" HP / "+ std::to_string((int)players.at(2)->GetMP()) +" MP";
			player3 = std::wstring(player3_s.length(), L' ');
			std::copy(player3_s.begin(), player3_s.end(), player3.begin());
		}
		else player3 = L"PLAYER 3 NOT CONNECTED";

		rect = vector4di(0, menuSize.Y/8*3, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player3, true, false, NETDEBUG_M_P3, netDebugWindow);
		
		// ##################################################
		
		if(players.size() > 3 && players.at(3) != NULL){
			std::string player4_s = players.at(3)->GetName() + ": " + std::to_string((int)players.at(3)->GetHP()) +" HP / "+ std::to_string((int)players.at(3)->GetMP()) +" MP";
			player4 = std::wstring(player4_s.length(), L' ');
			std::copy(player4_s.begin(), player4_s.end(), player4.begin());
		}
		else player4 = L"PLAYER 4 NOT CONNECTED";

		rect = vector4di(0, menuSize.Y/8*4, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player4, true, false, NETDEBUG_M_P4, netDebugWindow);
		
		// ##################################################
		
		if(players.size() > 4 && players.at(4) != NULL){
			std::string player5_s = players.at(4)->GetName() + ": " + std::to_string((int)players.at(4)->GetHP()) +" HP / "+ std::to_string((int)players.at(4)->GetMP()) +" MP";
			player5 = std::wstring(player5_s.length(), L' ');
			std::copy(player5_s.begin(), player5_s.end(), player5.begin());
		}
		else player5 = L"PLAYER 5 NOT CONNECTED";

		rect = vector4di(0, menuSize.Y/8*5, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player5, true, false, NETDEBUG_M_P5, netDebugWindow);
		
		// ##################################################
		
		if(players.size() > 5 && players.at(5) != NULL){
			std::string player6_s = players.at(5)->GetName() + ": " + std::to_string((int)players.at(5)->GetHP()) +" HP / "+ std::to_string((int)players.at(5)->GetMP()) +" MP";
			player6 = std::wstring(player6_s.length(), L' ');
			std::copy(player6_s.begin(), player6_s.end(), player6.begin());
		}
		else player6 = L"PLAYER 6 NOT CONNECTED";

		rect = vector4di(0, menuSize.Y/8*6, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player6, true, false, NETDEBUG_M_P6, netDebugWindow);
		
		// ##################################################
		
		if(players.size() > 6 && players.at(6) != NULL){
			std::string player7_s = players.at(6)->GetName() + ": " + std::to_string((int)players.at(6)->GetHP()) +" HP / "+ std::to_string((int)players.at(6)->GetMP()) +" MP";
			player7 = std::wstring(player7_s.length(), L' ');
			std::copy(player7_s.begin(), player7_s.end(), player7.begin());
		}
		else player7 = L"PLAYER 7 NOT CONNECTED";

		rect = vector4di(0, menuSize.Y/8*7, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player7, true, false, NETDEBUG_M_P7, netDebugWindow);
		
		// ##################################################
		
		if(players.size() > 7 && players.at(7) != NULL){
			std::string player8_s = players.at(7)->GetName() + ": " + std::to_string((int)players.at(7)->GetHP()) +" HP / "+ std::to_string((int)players.at(7)->GetMP()) +" MP";
			player8 = std::wstring(player8_s.length(), L' ');
			std::copy(player8_s.begin(), player8_s.end(), player8.begin());
		}
		else player8 = L"PLAYER 8 NOT CONNECTED";

		rect = vector4di(0, menuSize.Y/8*8, menuSize.X, menuSize.Y/8);
		g_engine->addStaticText(rect, player8, true, false, NETDEBUG_M_P8, netDebugWindow);
		
		// ##################################################

	}
}