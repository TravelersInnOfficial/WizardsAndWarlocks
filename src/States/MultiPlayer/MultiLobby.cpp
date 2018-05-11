#include "MultiLobby.h"
#include <GraphicEngine/MenuManager.h>

#include "./../Managers/NetworkManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Players/Player.h"
#include "./../LevelLoader.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GCamera.h>
#include <PhysicsEngine/BulletEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/NetworkObject.h>
#include <NetworkEngine/Server.h>

MultiLobby::MultiLobby(MultiPlayerGame* fat){
	father = fat;

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();
	n_engine 		= NetworkEngine::GetInstance();

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();
	
	// Level
	LevelLoader::LoadLevel("./../assets/json/Lobby2.json");

	if(n_engine->IsServerInit()){
		isServer = true;
		objectManager->SetWarlockSpawnSeed();
	}
	else if(n_engine->IsClientInit()) isServer = false;

	networkObject = networkManager->GetMultiGame();
	playerOne = playerManager->GetPlayerOne();

	// GraphicEngine
	if(playerOne==nullptr) g_engine->addCameraSceneNodeFPS(120.f, 0.005);

	// Ponemos a false el inicio de la partida de los players
	playerManager->ManageMatchStatus(false);

	if(networkObject){
		networkObject->SetBoolVar(MULTIGAME_CHANGE, false, false, false);
		networkObject->SetIntVar(MULTIGAME_WINNER_ALLIANCE, (int)NO_ALLIANCE, false, false);
	}

	g_engine->ToggleMenu(false);
}

MultiLobby::~MultiLobby(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

bool MultiLobby::Input(){
	return false;
}

void MultiLobby::UpdateLobby(float deltaTime){
	n_engine->Update(deltaTime, true);
	f_engine->UpdateWorld(deltaTime);

	if(g_engine->getActiveCamera() != nullptr){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}

	playerManager->UpdatePlayers(deltaTime, true);

	networkManager->Update();
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	trapManager->Update(deltaTime);
	
	playerManager->RespawnDeadPlayers();
	
	g_engine->UpdateReceiver();

	// Empezamos la partida si recibimos un TRUE en MULTIGAME_CHANGE (lo volvemos a poner a FALSE)
	if(networkObject != nullptr && networkObject->GetBoolVar(MULTIGAME_CHANGE)){
		networkObject->SetBoolVar(MULTIGAME_CHANGE, false, false, false);
		father->StartGame();
	}

	if(isServer){
		CheckIfReady();
		n_engine->GetServer()->SendSpawnerSeed();
	}
}

void MultiLobby::Update(float deltaTime){
	if(playerOne == nullptr && !isServer) playerOne = playerManager->GetPlayerOne();
	if(networkObject == nullptr) networkObject = networkManager->GetMultiGame();
	UpdateLobby(deltaTime);
}


void MultiLobby::Draw(){
	if(playerOne != nullptr){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->Draw();
	}
}

void MultiLobby::CheckIfReady(){
	// Comprobamos que todos los jugadores tengan su variable READY a true
	// Si la tienen, cargamos el siguente nivel
	if(isServer && playerManager->CheckIfReady()) networkObject->SetBoolVar(MULTIGAME_CHANGE, true, true, false);
}
