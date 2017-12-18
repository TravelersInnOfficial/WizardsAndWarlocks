#include "MenuManager.h"

MenuManager* MenuManager::instance = 0;

MenuManager::MenuManager(){
	g_engine = GraphicEngine::getInstance();
	irr::core::dimension2du res = g_engine->privateDriver->getScreenSize();
	screenSize.X = res.Width;
	screenSize.Y = res.Height;
}

MenuManager::~MenuManager(){
	ClearMenu();
}

MenuManager* MenuManager::GetInstance(){
	if(instance == 0) instance = new MenuManager();
	return instance;
}

void MenuManager::CreateMenu(MenuType type){
	switch(type){
		case(MAIN_M):{
			CreateMain();
			break;
		}
		case(ALLIANCE_M):{
			CreateAlliance();
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
}

void MenuManager::CreateMain(){

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30),
        false,
        L"Start your Game",
		0,
		MAIN_M_WINDOW
	);
	window->getCloseButton()->setVisible(false);
	window->setDraggable(false);
	loadedOptions.push_back(MAIN_M_WINDOW);

	vector4di rect = vector4di(screenSize.X/20.0f,screenSize.Y/3,screenSize.X/4.5f,screenSize.Y/3);
	g_engine->addButton(rect, L"Single Player", L"To play single player", MAIN_M_SINGLE, window);

	rect = vector4di(screenSize.X/1.70,screenSize.Y/3.75f,screenSize.X/9,screenSize.Y/30);
	g_engine->addStaticText(rect, L"Multi Player", true, false, MAIN_M_TEXT_1, window);

	rect = vector4di(screenSize.X/2.5f,screenSize.Y/3,screenSize.X/4.5f,screenSize.Y/3);
	g_engine->addButton(rect, L"Conect to Game", L"To start a client (FILL IP)", MAIN_M_CLIENT, window);

	rect = vector4di(screenSize.X/1.5f,screenSize.Y/3,screenSize.X/4.5f,screenSize.Y/3);
	g_engine->addButton(rect, L"Host a Game", L"To start a server on this computer", MAIN_M_SERVER, window);

	rect = vector4di(screenSize.X/2.5f,screenSize.Y/1.4f,screenSize.X/4.5f,screenSize.Y/30);
	g_engine->addEditBox(rect, L"127.0.0.1", MAIN_M_IP, window);
}

void MenuManager::CreateAlliance(){

 	irr::gui::IGUIWindow* window = g_engine->privateGUIEnv->addWindow(
        irr::core::rect<irr::s32>(screenSize.X/30,screenSize.Y/30,screenSize.X - screenSize.X/30, screenSize.Y - screenSize.Y/30),
        false,
        L"Select Alliance",
		0,
		ALLIANCE_M_WINDOW
	);
	window->setDraggable(false);
	window->getCloseButton()->setID(ALLIANCE_M_CLOSE);
	loadedOptions.push_back(ALLIANCE_M_WINDOW);

	vector4di rect = vector4di(screenSize.X/2-((screenSize.X/9.0f)/2),screenSize.Y/4,screenSize.X/9.0f,screenSize.Y/30);
	g_engine->addStaticText(rect, L"Select your Alliance, human.", true, false, MAIN_M_TEXT_1, window);
	
	rect = vector4di(screenSize.X/9.0f,screenSize.Y/3,screenSize.X/3.5f,screenSize.Y/3);
	g_engine->addButton(rect, L"Play as a Wizard", L"Be a Wizard and get back the Grail", MAIN_M_SINGLE, window);

	rect = vector4di(screenSize.X/1.67f,screenSize.Y/3,screenSize.X/3.5f,screenSize.Y/3);
	g_engine->addButton(rect, L"Play as a Warlock", L"Be a Warlock and protect the Grail", MAIN_M_CLIENT, window);
}