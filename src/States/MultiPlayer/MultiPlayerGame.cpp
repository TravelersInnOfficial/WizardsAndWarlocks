#include "MultiPlayerGame.h"

MultiPlayerGame::MultiPlayerGame(){
	// Motores
	g_engine		= GraphicEngine::getInstance();
	// Managers
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();
}

MultiPlayerGame::~MultiPlayerGame(){}

bool MultiPlayerGame::Input(){}

void MultiPlayerGame::Update(float deltaTime){}

void MultiPlayerGame::Draw(){}