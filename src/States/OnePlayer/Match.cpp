#include "Match.h"

Match::Match(SinglePlayerGame* fat){
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
	LevelLoader::LoadLevel("../assets/json/Map1.json");

	objectManager->AddNavmesh("./../assets/json/NavMesh.json");
	objectManager->AddRoomGraph( "./../assets/json/map_rooms.json");

	playerOne = playerManager->GetPlayerOne();
	if(playerOne == NULL){
		playerOne = playerManager->AddHumanPlayer(true);
	}

	// Ponemos a true el inicio de la partida
	playerManager->ManageMatchStatus(true);
}

Match::~Match(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

bool Match::Input(){

	return false;
}

void Match::Update(float deltaTime){
	f_engine->UpdateWorld();
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
	//playerManager->RespawnDeadPlayers();
	trapManager->Update(deltaTime);
	g_engine->UpdateReceiver();

	CheckIfWon();
}

void Match::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();

	if(playerOne != NULL){
		g_engine->drawAim(playerOne->GetMoving());
		//playerOne->DrawOverlays(deltaTime);
		playerOne->Draw();
		objectManager->DrawGrailGUI();
	}

	/*if(debug){
		f_engine->DebugDrawWorld();
		if(AL != NULL) AL->Debug();
	}*/
	
	g_engine->drawAllGUI();	// Draws the MENU (if one is activated)
	g_engine->endScene();
}

void Match::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = ALLIANCE_WIZARD;
	else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;

	if(whosWon != NO_ALLIANCE){
		GraphicEngine::getInstance()->InitReceiver();
		if(playerOne != NULL) {
			playerOne->SetAllInput(UP);

			//Play sound event when you lose or win
			//if (playerOne->GetAlliance() != whosWon) playEvent(soundEvents["defeat"]);
			//else playEvent(soundEvents["victory"]);

			//g_engine->ToggleMenu(true);
			//MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, whosWon);

			father->ReturnLobby();
		}
	}

}

