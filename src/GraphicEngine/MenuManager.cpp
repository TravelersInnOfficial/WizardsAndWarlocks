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
	window->getCloseButton()->setID(ALLIANCE_M_CLOSE);
	loadedOptions.push_back(ALLIANCE_M_WINDOW);

	vector4di rect = vector4di(menuSize.X/2-((menuSize.X/9.0f)/2),menuSize.Y/4,menuSize.X/9.0f,menuSize.Y/30);
	g_engine->addStaticText(rect, L"Select your Alliance, human.", true, false, ALLIANCE_M_TEXT_1, window);
	
	rect = vector4di(menuSize.X/9.0f,menuSize.Y/3,menuSize.X/3.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Play as a Wizard", L"Be a Wizard and get back the Grail", ALLIANCE_M_WIZARD, window);

	rect = vector4di(menuSize.X/1.67f,menuSize.Y/3,menuSize.X/3.5f,menuSize.Y/3);
	g_engine->addButton(rect, L"Play as a Warlock", L"Be a Warlock and protect the Grail", ALLIANCE_M_WARLOCK, window);
}