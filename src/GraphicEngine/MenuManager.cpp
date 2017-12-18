#include "MenuManager.h"

MenuManager* MenuManager::instance = 0;

MenuManager::MenuManager(){
	g_engine = GraphicEngine::getInstance();
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
	vector4di rect = vector4di(100,200,200,200);
	g_engine->addButton(rect, L"Single Player", L"To play single player", MAIN_M_SINGLE);
	loadedOptions.push_back(MAIN_M_SINGLE);

	rect = vector4di(575,160,100,20);
	g_engine->addStaticText(rect, L"Multi Player", true, false, MAIN_M_TEXT_1);
	loadedOptions.push_back(MAIN_M_TEXT_1);

	rect = vector4di(400,200,200,200);
	g_engine->addButton(rect, L"Conect to Game", L"To start a client (FILL IP)", MAIN_M_CLIENT);
	loadedOptions.push_back(MAIN_M_CLIENT);

	rect = vector4di(650,200,200,200);
	g_engine->addButton(rect, L"Host a Game", L"To start a server on this computer", MAIN_M_SERVER);
	loadedOptions.push_back(MAIN_M_SERVER);

	rect = vector4di(400,420,200,20);
	g_engine->addEditBox(rect, L"127.0.0.1", MAIN_M_IP);
	loadedOptions.push_back(MAIN_M_IP);
}