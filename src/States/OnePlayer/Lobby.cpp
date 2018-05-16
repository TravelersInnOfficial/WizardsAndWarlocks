#include "Lobby.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"
#include "./../LevelLoader.h"
#include "./../Players/Player.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GCamera.h>
#include <PhysicsEngine/BulletEngine.h>
#include <SoundEngine/SoundSystem.h>

Lobby::Lobby(SinglePlayerGame* fat){
	father = fat;

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	senseManager	= RegionalSenseManager::GetInstance();

	objectManager->SetWarlockSpawnSeed();

	// Level
	LevelLoader::LoadLevel("./../assets/json/Lobby2.json");

	playerOne = playerManager->GetPlayerOne();
	if(playerOne == nullptr) playerOne = playerManager->AddHumanPlayer(true);

	// Ponemos a false el inicio de la partida de los players
	playerManager->ManageMatchStatus(false);
}

Lobby::~Lobby(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

#include "./../../Players/AIPlayer.h"

bool Lobby::Input(){
	if(g_engine->IsKeyPressed(Key_V)){
		playerManager->AddAIPlayer();
	}

	if(g_engine->IsKeyPressed(Key_B)){
		AIPlayer* p = playerManager->AddAIPlayer();
		p->SetAlliance(ALLIANCE_WARLOCK);
	}

	return false;
}

void Lobby::Update(float deltaTime){
	f_engine->UpdateWorld(deltaTime);
	if(g_engine->getActiveCamera() != nullptr) s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	senseManager->CreateAllSignals();			// Creamos todas las senyales (visuales, sonoras) del juego
	senseManager->SendSignals();				// Update de las notificaciones sensoriales
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	playerManager->UpdatePlayers(deltaTime);
	playerManager->RespawnDeadPlayers();
	trapManager->Update(deltaTime);
	g_engine->UpdateReceiver();
	
	CheckIfReady();
}

void Lobby::Draw(){

	if(playerOne != nullptr){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->Draw();
	}

}

void Lobby::CheckIfReady(){
	// Comprobamos que el jugador uno este dentro de la zona
	if(playerOne == nullptr) return;
	playerOne->CheckIfReady();

	// Si esta dentro de la zona, cargamos el siguiente nivel
	if(playerOne->GetReadyStatus()) father->StartGame();
}
