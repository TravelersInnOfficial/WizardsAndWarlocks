#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <vector3d.h>
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "NetworkEngine/NetworkEngine.h"
#include "./States/Game.h"
#include "./States/NetGame.h"
#include "./Managers/ObjectManager.h"
#include "./LevelLoader.h"

NetworkEngine* CreateMenu(){
	NetworkEngine* n_engine = NULL;
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	vector4di rect = vector4di(100,200,200,200);
	g_engine->addButton(rect, L"Single Player");

	rect = vector4di(575,160,100,20);
	g_engine->addStaticText(rect, L"Multi Player", true, false);

	rect = vector4di(400,200,200,200);
	g_engine->addButton(rect, L"Client");

	rect = vector4di(650,200,200,200);
	g_engine->addButton(rect, L"Server");

	rect = vector4di(400,420,200,20);
	g_engine->addEditBox(rect, L"Server IP");

	int draw = 120;
	while(draw > 0){
		g_engine->beginSceneDefault();
		g_engine->drawAllGUI();
		g_engine->endScene();
		draw--;
	}

	return (n_engine);
}

NetworkEngine* manageArguments(int argc, char* argv[]){
	NetworkEngine* n_engine = NULL;
	if(argc > 1){
		n_engine = NetworkEngine::GetInstance();
		bool isServer = false;
		if(strcmp(argv[1],"-s") == 0) isServer = true;
		if (isServer) n_engine->StartServer();
		else {
			if(argc > 2) n_engine->SetIp(argv[2]);
			n_engine->StartClient();
		}
	}
	return (n_engine);
}

int main(int argc, char* argv[]) {

	NetworkEngine* n_engine = manageArguments(argc, argv);

	// Engines
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	// GUI
	NetworkEngine* n_engine2 = CreateMenu();
	g_engine->setCursorVisible(false);

	// Level
	LevelLoader loader;
	loader.loadLobby();

	// Game State
	if (n_engine == NULL){
		Game* game = new Game();
		bool end = false;
		while(g_engine->run() && !end){
			end = game->Input();
			game->Update();
			game->Draw();
			std::this_thread::sleep_for(std::chrono::milliseconds(7));
			g_engine->endScene();	
		}
		delete game;
	}

	// Network Game State
	else{
		NetGame* game = NetGame::GetInstance();
		bool end = false;
		while(g_engine->run() && !end){
			end = game->Input();
			game->Update();
			game->Draw();
			std::this_thread::sleep_for(std::chrono::milliseconds(7));
			g_engine->endScene();	
		}
		delete game;
	}

	// End Engines
	if(n_engine != NULL) n_engine->EndService();
	f_engine->EraseWorld();
	g_engine->drop();

	return 0;
}