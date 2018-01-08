#include "Game.h"
#include "./../Objects/DamageArea.h"

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
	LevelLoader loader;
	loader.LoadLevel("../assets/json/Lobby.json");
	lobbyState = true;
	gameEnded = false;
	secondCounter = 0;

	// Sound Engine
	s_engine->createSystem("./../assets/banks/");

	// Graphic Engine
	timeStart = g_engine->getTime() * 0.001;

	// Jugador
	playerOne = (HumanPlayer*) playerManager->AddHumanPlayer();
	spellManager->AddHechizo(0, playerOne, SPELL_PROJECTILE);
	spellManager->AddHechizo(1, playerOne, SPELL_FIRE);
	spellManager->AddHechizo(2, playerOne, SPELL_WALL);
	spellManager->AddHechizo(3, playerOne, SPELL_BLIZZARD);
	//effectManager->AddEffect(playerOne, WEAK_MADNESS);

	AL = playerManager->AddAIPlayer();
	spellManager->AddHechizo(0, AL, SPELL_PROJECTILE);
	spellManager->AddHechizo(1, AL, SPELL_FIRE);
	spellManager->AddHechizo(2, AL, SPELL_WALL);
	spellManager->AddHechizo(3, AL, SPELL_BLIZZARD);
}

Game::~Game(){
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete senseManager;
}

bool Game::Input(){
	bool end = false;
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) end = true;

	if(g_engine->IsKeyPressed(KEY_KEY_I)) playerOne->ChangeHP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+30);
	
	if(g_engine->IsKeyPressed(KEY_KEY_K)) playerOne->ChangeMP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_L)) playerOne->ChangeMP(+30);
	
	if(g_engine->IsKeyPressed(KEY_KEY_R)) playerOne->Respawn();
	if(g_engine->IsKeyPressed(KEY_KEY_M)) playerOne->changeSurface(1.0f);
	if(g_engine->IsKeyPressed(KEY_KEY_N)) playerOne->changeSurface(0.0f);

	if(gameEnded){
		int option = g_engine->ReadButtonPressed();
		if(option == ENDMATCH_M_CONFIRM) RestartMatch();
	}

	return end;
}

void Game::Update(){
	
	UpdateDelta();

	f_engine->UpdateWorld();
	
	if(g_engine->getActiveCamera() != NULL){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}

	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	playerManager->UpdatePlayers();
	playerManager->RespawnDeadPlayers();
	trapManager->Update(deltaTime);

	g_engine->UpdateReceiver();
	senseManager->SendSignals();

	setFps();

	// START/END MATCH
	if(lobbyState) CheckIfReady();
	else if (!gameEnded) CheckIfWon();

}

void Game::CheckIfReady(){
	// Comprobamos que el jugador uno este dentro de la zona
	playerOne->CheckIfReady();

	// Si esta dentro de la zona, cargamos el siguiente nivel
	if(playerOne->GetReadyStatus()) {
		LevelLoader loader;
		loader.LoadLevel("../assets/json/map.json");
		lobbyState = false;
		playerManager->ManageMatchStatus(true);
		g_engine->ToggleMenu(false);
		MenuManager::GetInstance()->ClearMenu();
	}
}

void Game::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim(playerOne->GetMoving());
	if(playerOne != NULL) playerOne->DrawOverlays(deltaTime);
	if(playerOne != NULL) g_engine->drawManaAndHealth(playerOne->GetHP(), playerOne->GetMP());
	//f_engine->DebugDrawWorld();
	if(AL != NULL) AL->Debug();
	GraphicEngine::getInstance()->drawAllGUI();	// Draws the MENU (if one is activated)
}

void Game::setFps(){
	secondCounter += deltaTime;
	if(secondCounter >= 0.5){
		secondCounter = 0;
		std::string myFps = to_string(int(1/deltaTime));
		std::wstring wsTmp(myFps.begin(), myFps.end());
		g_engine->ChangeWindowName(wsTmp);
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
	LevelLoader loader;
	loader.LoadLevel("../assets/json/Lobby.json");
	MenuManager::GetInstance()->ClearMenu();
	g_engine->ToggleMenu(false);
	playerManager->ManageMatchStatus(false);
	playerManager->ReturnAllToLobby();
}