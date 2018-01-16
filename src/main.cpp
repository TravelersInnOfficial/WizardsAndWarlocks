#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <vector3d.h>
#include <Menus.h>
#include <StateCodes.h>
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "GraphicEngine/MenuManager.h"
#include "NetworkEngine/NetworkEngine.h"
#include "SoundEngine/SoundSystem.h"
#include "./Managers/StateManager.h"
#include "./Managers/ObjectManager.h"

int main() {

	// Engines
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	SoundSystem* s_engine = SoundSystem::getInstance();
	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	s_engine->createSystem("./../assets/banks/");

	// MAIN MENU
	StateManager* state = StateManager::GetInstance();
	while(state->Update()){}

	delete state;
	delete f_engine;
	delete g_engine;
	delete s_engine;
	delete n_engine;
	
	return 0;
}