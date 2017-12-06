#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <vector3d.h>
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "./Game.h"
#include "./Managers/ObjectManager.h"
#include "./LevelLoader.h"

int main() {

	// Engines
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	g_engine->setCursorVisible(false);
	g_engine->addCameraSceneNodeFPS(120.f, 0.f);

	// Level
	LevelLoader loader;
	loader.loadLobby();

	// Game State
	Game* game = Game::GetInstance();
	bool end = false;
	while(g_engine->run() && !end){
		end = game->Input();
		game->Update();
		game->Draw();
		//std::this_thread::sleep_for(std::chrono::milliseconds(7));
		g_engine->endScene();	
	}
	delete game;

	// End Engines
	f_engine->EraseWorld();
	g_engine->drop();

	return 0;
}