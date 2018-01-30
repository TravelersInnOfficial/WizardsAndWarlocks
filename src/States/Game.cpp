#include "Game.h"
#include "./../Managers/StateManager.h"
#include <StateCodes.h>

Game::Game(){

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	senseManager	= RegionalSenseManager::GetInstance();

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();

	// Level
	LevelLoader::LoadLevel("./../assets/json/Lobby2.json");
	lobbyState = true;
	gameEnded = false;
	debug = false;
	mute = false;
	captured = false;
	secondCounter = 0;

	//NavMesh
	//objectManager->AddNavmesh("./../assets/json/NavMesh.json");

	// Sound Engine
	createSoundEvents();

	// Graphic Engine
	timeStart = g_engine->getTime() * 0.001;

	// Jugador
	playerOne = (HumanPlayer*) playerManager->AddHumanPlayer();

	//AL = NULL;
	AL = playerManager->AddAIPlayer();
	AL->SetAlliance(ALLIANCE_WARLOCK);
 	playerManager->AddAIPlayer();
	playEvent(soundEvents["ghosts"], vector3df(-0.245, 1.14, 17.25));
	playEvent(soundEvents["waterdrops"], vector3df(-0.245, 1.20, 17.25));

}

Game::~Game(){
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete spellManager;	// Tiene que eliminarse despues de el player
	delete senseManager;

	std::map<std::string, SoundEvent*>::iterator it = soundEvents.begin();
	for(; it!=soundEvents.end(); it++){
		SoundEvent* even = it->second;
		even->release();
	}
}

bool Game::Input(){
	
	if(!g_engine->run()){
		return true;
	}

	// SALIR
	if(g_engine->IsKeyPressed(KEY_ESCAPE)){
		StateManager::GetInstance()->PrepareStatus(STATE_MENU);
	}

	// TEMPORALES
	if(g_engine->IsKeyPressed(KEY_KEY_I)) playerOne->ChangeHP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+30);
	if(g_engine->IsKeyPressed(KEY_KEY_K)) playerOne->ChangeMP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_L)) AL->ShortestPath(playerOne->GetPos());
	if(g_engine->IsKeyPressed(KEY_KEY_P)) playerOne->Respawn();

	// DEBUG
	if(g_engine->IsKeyPressed(KEY_F1)) debug = !debug;

	if(g_engine->IsKeyPressed(KEY_F2)){
		float vol = 1;
		if(!mute) vol = 0;
		SoundSystem::getInstance()->setVolume(vol);
		mute = !mute;
	}

	if(g_engine->IsKeyPressed(KEY_F3)){
		g_engine->ToggleCameraMovement(captured);
		g_engine->setCursorVisible(!captured);
		captured = !captured;
	}

	// SONIDO
	//if(g_engine->IsKeyPressed(KEY_KEY_M)) playerOne->changeSurface(4.0f);
	//if(g_engine->IsKeyPressed(KEY_KEY_N)) playerOne->changeSurface(0.0f);

	if(gameEnded){
		int option = g_engine->ReadButtonPressed();
		if(option == ENDMATCH_M_CONFIRM) RestartMatch();
	}

	return false;
}

void Game::Update(){

	UpdateDelta();

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
	playerManager->UpdatePlayers();
	//playerManager->RespawnDeadPlayers();
	trapManager->Update(deltaTime);

	g_engine->UpdateReceiver();
	

	setFps();

	// START/END MATCH
	if(lobbyState) CheckIfReady();
	else if (!gameEnded) CheckIfWon();
}

void Game::CheckIfReady(){
	// Comprobamos que el jugador uno este dentro de la zona
	if(playerOne == NULL) return;
	playerOne->CheckIfReady();

	// Si esta dentro de la zona, cargamos el siguiente nivel
	if(playerOne->GetReadyStatus()) {
		LevelLoader::LoadLevel("../assets/json/Map1.json");

		objectManager->AddNavmesh("./../assets/json/NavMesh.json");
	
		lobbyState = false;
		playerManager->ManageMatchStatus(true);
		g_engine->ToggleMenu(false);
		MenuManager::GetInstance()->ClearMenu();
	}

}

void Game::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	if(playerOne != NULL){
	g_engine->drawAim(playerOne->GetMoving());
		playerOne->DrawOverlays(deltaTime);
		playerOne->Draw();
		objectManager->DrawGrailGUI();
	}
	if(debug){
		f_engine->DebugDrawWorld();
		if(AL != NULL) AL->Debug();
	}
	
	g_engine->drawAllGUI();	// Draws the MENU (if one is activated)
	g_engine->endScene();
}

void Game::setFps(){
	secondCounter += deltaTime;
	if(secondCounter >= 0.5){
		secondCounter = 0;
		std::string myFps = to_string(int(1/deltaTime));
		std::wstring wsTmp(myFps.begin(), myFps.end());
		g_engine->ChangeWindowName(L"Wizards And Warlocks Master v1.0 FPS: " + wsTmp);
	}
}

float Game::GetTotalTime(){ return GraphicEngine::getInstance()->getTime(); }

float Game::GetDeltaTime(){ return deltaTime; }

void Game::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}

void Game::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = ALLIANCE_WIZARD;
	else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;

	if(whosWon != NO_ALLIANCE){
		GraphicEngine::getInstance()->InitReceiver();
		gameEnded = true;
		if(playerOne != NULL) {
			playerOne->SetAllInput(UP);

			//Play sound event when you lose or win
			if (playerOne->GetAlliance() != whosWon) playEvent(soundEvents["defeat"]); else playEvent(soundEvents["victory"]);

			playerManager->EraseAllCharacters();
			g_engine->ToggleMenu(true);
			MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, whosWon);
		}
		else RestartMatch();
	}

}

void Game::RestartMatch(){
	gameEnded = false;
	lobbyState = true;
	LevelLoader::LoadLevel("../assets/json/Lobby2.json");
	MenuManager::GetInstance()->ClearMenu();
	g_engine->ToggleMenu(false);
	playerManager->ManageMatchStatus(false);
	playerManager->ReturnAllToLobby();
}
/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void Game::createSoundEvents() {
	//Create the sound events
	SoundEvent * defeat  = SoundSystem::getInstance()->createEvent("event:/Music/Defeat");
	SoundEvent * victory = SoundSystem::getInstance()->createEvent("event:/Music/Victory");
	SoundEvent * ghosts  = SoundSystem::getInstance()->createEvent("event:/Ambience/Ghosts");
	SoundEvent * waterDrops  = SoundSystem::getInstance()->createEvent("event:/Ambience/WaterDrops");

	//Store them at the map
	soundEvents["defeat"]  = defeat;
	soundEvents["victory"] = victory;
	soundEvents["ghosts"]  = ghosts;
	soundEvents["waterdrops"]  = waterDrops;
}
void Game::playEvent(SoundEvent* event, vector3df pos) {
	SoundSystem::getInstance()->playEvent(event, pos);
}

void Game::playEvent(SoundEvent* event) {
	SoundSystem::getInstance()->playEvent(event);
}
