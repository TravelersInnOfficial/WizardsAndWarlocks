#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <vector3d.h>
#include <Menus.h>
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "GraphicEngine/MenuManager.h"
#include "NetworkEngine/NetworkEngine.h"
#include "SoundEngine/SoundSystem.h"
#include "./States/Game.h"
#include "./States/NetGame.h"
#include "./Managers/ObjectManager.h"

NetworkEngine* SpawnMenu(){
	NetworkEngine* n_engine = NULL;
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	

	MenuManager::GetInstance()->CreateMenu(MAIN_M);

	MenuOption selectedOption = NO_OPT;
	while(g_engine->run() && selectedOption == NO_OPT){
		g_engine->beginSceneDefault();
		g_engine->drawAllGUI();
		g_engine->endScene();
		selectedOption = g_engine->ReadButtonPressed();
	}
	
	switch(selectedOption){
		case MAIN_M_CLIENT:{
			n_engine = NetworkEngine::GetInstance();
			n_engine->SetIp(g_engine->ReadText(MAIN_M_IP));
			n_engine->StartClient();
			n_engine->GetClient()->SetClientName(g_engine->ReadText(MAIN_M_NAME));
			break;
		}
		case MAIN_M_SERVER:{
			n_engine = NetworkEngine::GetInstance();
			n_engine->StartServer();
			break;
		}
		default:{
			break;
		}
	}

	MenuManager::GetInstance()->ClearMenu();
	g_engine->ToggleMenu(false);
	return (n_engine);
}

int main() {

	// Engines
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	SoundSystem* s_engine = SoundSystem::getInstance();
	s_engine->createSystem("./../assets/banks/");

	// MAIN MENU
	NetworkEngine* n_engine = SpawnMenu();
	g_engine->setCursorVisible(false);

	// Game State
	if (n_engine == NULL && g_engine->run()){
		Game* game = new Game();
		bool end = false;
		while(g_engine->run() && !end){
			end = game->Input();
			game->Update();
			game->Draw();
			g_engine->endScene();	
		}
		delete game;
	}

	// Network Game State
	else if(g_engine->run()){
		NetGame* game = NetGame::GetInstance();
		bool end = false;
		while(g_engine->run() && !end){
			end = game->Input();
			game->Update();
			game->Draw();
			g_engine->endScene();			
		}
		delete game;
	}

	// End Engines
	if(n_engine != NULL) n_engine->EndService();
	f_engine->EraseWorld();
	g_engine->drop();
	delete f_engine;
	delete g_engine;
	delete s_engine;
	
	return 0;
}