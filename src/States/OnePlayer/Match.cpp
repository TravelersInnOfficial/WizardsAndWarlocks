#include "Match.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Players/Player.h"
#include "./../LevelLoader.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GCamera.h>
#include <GraphicEngine/MenuManager.h>
#include <PhysicsEngine/BulletEngine.h>
#include <SoundEngine/SoundSystem.h>

Match::Match(SinglePlayerGame* fat){
	father = fat;

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();

	senseManager	= RegionalSenseManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	spellManager 	= SpellManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	
	// Level
	LevelLoader::LoadLevel("../assets/json/castle_final.json");
	objectManager->AddNavmesh("./../assets/json/NavMesh.json");
	objectManager->AddRoomGraph( "./../assets/json/map_rooms.json");

	objectManager->SetWarlockSpawn();

	playerOne = playerManager->GetPlayerOne();

	// Ponemos a true el inicio de la partida
	playerManager->ManageMatchStatus(true);
	gameEnded = false;

	father->PlayEvent("ghosts", vector3df(-0.245, 1.14, 17.25));
	father->PlayEvent("waterdrops", vector3df(-0.245, 1.20, 17.25));
}

Match::~Match(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

bool Match::Input(){

	if(gameEnded){/*
		int option = g_engine->ReadButtonPressed();
		if(option == ENDMATCH_M_CONFIRM) */
		if(MenuManager::GetInstance()->GetCurrentMenu() == nullptr) father->ReturnLobby();
	}

	if(g_engine->IsKeyPressed(Key_M)){
		std::cout<<playerOne->GetPos()<<std::endl;
	}

	return false;
}

void Match::Update(float deltaTime){
	f_engine->UpdateWorld(deltaTime);
	if(g_engine->getActiveCamera() != nullptr){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}
	senseManager->CreateAllSignals();			// Creamos todas las senyales (visuales, sonoras) del juego
	senseManager->SendSignals();				// Update de las notificaciones sensoriales
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	playerManager->UpdatePlayers(deltaTime);
	//playerManager->RespawnDeadPlayers();
	trapManager->Update(deltaTime);
	g_engine->UpdateReceiver();

	CheckIfWon();
}

void Match::Draw(){

	if(playerOne != nullptr){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->Draw();
		playerManager->DrawDebug();
	}
}

void Match::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = ALLIANCE_WIZARD;
	else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;

	if(whosWon != NO_ALLIANCE){
		GraphicEngine::getInstance()->InitReceiver();
		gameEnded = true;
		if(playerOne != nullptr) {
			playerOne->SetAllInput(UP);

			//Play sound event when you lose or win
			if (playerOne->GetAlliance() != whosWon) father->PlayEvent("defeat");
			else father->PlayEvent("victory");

			g_engine->ToggleCameraMovement(false);
			g_engine->ToggleMenu(true);
			MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, whosWon);
		}
	}

}
