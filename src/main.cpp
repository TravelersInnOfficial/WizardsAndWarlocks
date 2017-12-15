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
	g_engine->addButton(rect, L"Single Player", L"To play single player", 1);

	rect = vector4di(575,160,100,20);
	g_engine->addStaticText(rect, L"Multi Player", true, false);

	rect = vector4di(400,200,200,200);
	g_engine->addButton(rect, L"Client", L"To start a client (FILL IP)", 2);

	rect = vector4di(650,200,200,200);
	g_engine->addButton(rect, L"Server", L"To start a server on this computer", 3);

	rect = vector4di(400,420,200,20);
	g_engine->addEditBox(rect, L"127.0.0.1", 4);

	int selectedOption = -1;
	while(g_engine->run() && selectedOption == -1){
		g_engine->beginSceneDefault();
		g_engine->drawAllGUI();
		g_engine->endScene();

		selectedOption = g_engine->ReadMenu();
	}

	switch(selectedOption){
		case 2:{
			n_engine = NetworkEngine::GetInstance();
			n_engine->SetIp(g_engine->ReadText(4));
			n_engine->StartClient();
			break;
		}
		case 3:{
			n_engine = NetworkEngine::GetInstance();
			n_engine->StartServer();
			break;
		}
	}

	return (n_engine);
}

int main() {

	// Engines
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	// MAIN MENU
	NetworkEngine* n_engine = CreateMenu();
	g_engine->setCursorVisible(false);

	// Level
	LevelLoader loader;
	loader.LoadLevel("../assets/json/lobby.json");

	// Game State
	if (n_engine == NULL){
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
	else{
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

	return 0;
}