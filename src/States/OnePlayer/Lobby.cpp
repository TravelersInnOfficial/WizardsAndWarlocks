#include "Lobby.h"

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

	// Level
	LevelLoader::LoadLevel("./../assets/json/Lobby2.json");

	playerOne = playerManager->GetPlayerOne();
	if(playerOne == NULL){
		playerOne = playerManager->AddHumanPlayer(true);
	}

	// Ponemos a false el inicio de la partida de los players
	playerManager->ManageMatchStatus(false);
	
}

Lobby::~Lobby(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

bool Lobby::Input(){
	if(g_engine->IsKeyPressed(KEY_KEY_V)){
		playerManager->AddAIPlayer();
	}

	if(g_engine->IsKeyPressed(KEY_KEY_B)){
		Player* p = playerManager->AddAIPlayer();
		p->SetAlliance(ALLIANCE_WARLOCK);
	}

	return false;
}

void Lobby::Update(float deltaTime){
	f_engine->UpdateWorld(deltaTime);
	if(g_engine->getActiveCamera() != NULL){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}
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

	if(playerOne != NULL){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->Draw();
	}

}

void Lobby::CheckIfReady(){
	// Comprobamos que el jugador uno este dentro de la zona
	if(playerOne == NULL) return;
	playerOne->CheckIfReady();

	// Si esta dentro de la zona, cargamos el siguiente nivel
	if(playerOne->GetReadyStatus()) father->StartGame();
}

