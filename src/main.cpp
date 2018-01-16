#include <iostream>
#include "./Managers/StateManager.h"
#include "./PhysicsEngine/BT_Body.h"
#include "./GraphicEngine/GraphicEngine.h"
#include "./NetworkEngine/NetworkEngine.h"
#include "./SoundEngine/SoundSystem.h"

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