#include "./MenuPrincipal.h"
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <GraphicEngine/MenuManager.h>

MenuPrincipal::MenuPrincipal(){
	g_engine = GraphicEngine::getInstance();
	g_engine->setCursorVisible(true);
	g_engine->ToggleMenu(true);
	g_engine->InitReceiver();
	selectedOption = NO_OPT;
	MenuManager::GetInstance()->CreateMenu(MAIN_M);
}

MenuPrincipal::~MenuPrincipal(){
	g_engine->setCursorVisible(false);
}

bool MenuPrincipal::Input(){
	if(!g_engine->run()){
		return true;
	}

	if(g_engine->EscPressed()){
		return true;
	}

	selectedOption = g_engine->ReadButtonPressed();
	return false;
}

void MenuPrincipal::Update(){
	SoundSystem::getInstance()->Update();
	// En el caso de que se haya cambiado de opcion
	if(selectedOption != NO_OPT){
		// Dependiendo de la seleccion hacemos una cosa u otra
		switch(selectedOption){
			case MAIN_M_CLIENT:{
				PrepareClient();
				break;
			}
			case MAIN_M_SERVER:{
				PrepareServer();
				break;
			}
			default:{
				PrepareGame();
				break;
			}
		}
		MenuManager::GetInstance()->ClearMenu();
		g_engine->ToggleMenu(false);
	}
}

void MenuPrincipal::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAllGUI();
	g_engine->endScene();
}

void MenuPrincipal::PrepareGame(){
	StateManager::GetInstance()->PrepareStatus(STATE_GAME);
}

void MenuPrincipal::PrepareClient(){
	NetworkEngine* n_engine;
	n_engine = NetworkEngine::GetInstance();
	n_engine->SetIp(g_engine->ReadText(MAIN_M_IP));
	n_engine->StartClient();
	n_engine->GetClient()->SetClientName(g_engine->ReadText(MAIN_M_NAME));
	StateManager::GetInstance()->PrepareStatus(STATE_NETGAME);
}

void MenuPrincipal::PrepareServer(){
	NetworkEngine* n_engine;
	n_engine = NetworkEngine::GetInstance();
	n_engine->StartServer();
	StateManager::GetInstance()->PrepareStatus(STATE_NETGAME);
}

