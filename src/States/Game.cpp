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
	footstepEvent = s_engine->getEvent("event:/Character/Hard/Footsteps");

	// Graphic Engine
	timeStart = g_engine->getTime() * 0.001;

	// Jugador
	playerOne = (HumanPlayer*) playerManager->AddHumanPlayer();
	spellManager->AddHechizo(0, playerOne, SPELL_PROYECTIL);
	spellManager->AddHechizo(1, playerOne, SPELL_BASIC);
	spellManager->AddHechizo(2, playerOne, SPELL_DESPERIATONMURI);
	spellManager->AddHechizo(3, playerOne, SPELL_GUIVERNUMVENTUS);
	
	AL = playerManager->AddAIPlayer();
	spellManager->AddHechizo(0, AL, SPELL_PROYECTIL);
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
	if(g_engine->IsKeyPressed(KEY_KEY_F)) playerOne->DeployTrap();
	if(g_engine->IsKeyPressed(KEY_KEY_P)) playerOne->ChangeHP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+3);
	if(g_engine->IsKeyPressed(KEY_KEY_R)) playerOne->Respawn();

	if(g_engine->IsKeyPressed(KEY_KEY_A) || g_engine->IsKeyPressed(KEY_KEY_W) || g_engine->IsKeyPressed(KEY_KEY_S) || g_engine->IsKeyPressed(KEY_KEY_D)){
		if(!footstepEvent->isPlaying()){ footstepEvent->setPosition(playerOne->GetPos());footstepEvent->start();}
		s_engine->setListenerPosRot(playerOne->GetPos(), playerOne->GetRot());
		
	}
	else if (g_engine->IsKeyUp(KEY_KEY_A) && g_engine->IsKeyUp(KEY_KEY_W) && g_engine->IsKeyUp(KEY_KEY_S) && g_engine->IsKeyUp(KEY_KEY_D)){
		if(footstepEvent->isPlaying()) footstepEvent->stop();
	}

	if(g_engine->IsKeyPressed(KEY_KEY_M)) footstepEvent->setParamValue("Surface", 1.0f);
	if(g_engine->IsKeyPressed(KEY_KEY_N)) footstepEvent->setParamValue("Surface", 0.0f);

	if(gameEnded){
		int option = g_engine->ReadButtonPressed();
		if(option == ENDMATCH_M_CONFIRM) RestartMatch();
	}

	return end;
}

void Game::Update(){
	UpdateDelta();

	f_engine->UpdateWorld();
	s_engine->update();

	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	playerManager->UpdatePlayers();
	trapManager->Update(deltaTime);

	g_engine->UpdateReceiver();
	senseManager->SendSignals();

	setFps();

	// START MATCH
	if(lobbyState){
		if(playerManager->CheckIfReady()) {
			LevelLoader loader;
			loader.LoadLevel("../assets/json/map.json");
			lobbyState = false;
			playerManager->ManageMatchStatus(true);
			g_engine->ToggleMenu(false);
			MenuManager::GetInstance()->ClearMenu();
		}
	}
	else if (!gameEnded) CheckIfWon();

}

void Game::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim();
	g_engine->drawOverlays();
	if(playerOne != NULL) g_engine->drawManaAndHealth(playerOne->GetHP(), playerOne->GetMP());
	//f_engine->DebugDrawWorld();
	AL->Debug();
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
	int whosWon = -1;

	if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = 0;
	else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = 1;

	if(whosWon != -1){
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