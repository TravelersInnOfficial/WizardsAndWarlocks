#include "NetGame.h"

NetGame* NetGame::instance = NULL;

NetGame* NetGame::GetInstance(){
	if(instance == NULL) instance = new NetGame();
	return instance;
}

NetGame::NetGame(){

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();
	n_engine 		= NetworkEngine::GetInstance();

	// Level
	LevelLoader loader;
	loader.LoadLevel("../assets/json/Lobby.json");
	lobbyState = true;
	gameEnded = false;
	secondCounter = 0;

	if(n_engine->IsServerInit()) isServer = true;
	else if(n_engine->IsClientInit()) isServer = false;

	// Sound Engine
	s_engine->createSystem("./../assets/banks/");
	footstepEvent = s_engine->getEvent("event:/Character/Hard/Footsteps");

	// Graphic Engine
	timeStart = g_engine->getTime() * 0.001;
	g_engine->addCameraSceneNodeFPS(120.f, 0.005);

	// Jugador
	playerOne = NULL;
}

NetGame::~NetGame(){
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete networkManager;
}

bool NetGame::Input(){
	bool end = false;
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) end = true;
	
	if (g_engine->IsKeyUp(KEY_KEY_A) && g_engine->IsKeyUp(KEY_KEY_W) && g_engine->IsKeyUp(KEY_KEY_S) && g_engine->IsKeyUp(KEY_KEY_D)){
		if(footstepEvent->isPlaying()) footstepEvent->stop();
	}

	if(playerOne != NULL){
		/*if(g_engine->IsKeyPressed(KEY_KEY_F)) playerOne->DeployTrap();
		if(g_engine->IsKeyPressed(KEY_KEY_P)) playerOne->ChangeHP(-5);
		if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+3);
		if(g_engine->IsKeyDown(KEY_KEY_R)) playerOne->Respawn();*/
		if(g_engine->IsKeyPressed(KEY_KEY_A) || g_engine->IsKeyPressed(KEY_KEY_W) || g_engine->IsKeyPressed(KEY_KEY_S) || g_engine->IsKeyPressed(KEY_KEY_D)){
			if(!footstepEvent->isPlaying()) footstepEvent->start();
		}
	}

	if(gameEnded){
		int option = g_engine->ReadButtonPressed();
		if(option == ENDMATCH_M_CONFIRM) RestartMatch();
	}

	return end;
}

void NetGame::Update(){
	UpdateDelta();

	n_engine->Update();
	f_engine->UpdateWorld();
	s_engine->update();

	networkManager->Update();
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	trapManager->Update(deltaTime);

	playerManager->UpdatePlayers(true);
	g_engine->UpdateReceiver();

	setFps();

	// START/END MATCH
	if(lobbyState) CheckIfReady();
	else if (!gameEnded) CheckIfWon();

}

void NetGame::CheckIfReady(){
	// Comprobamos que el jugador uno este dentro de la zona
	if(playerOne != NULL) playerOne->CheckIfReady();

	// Comprobamos que todos los jugadores tengan su variable READY a true
	// Si la tienen, cargamos el siguente nivel
	if(playerManager->CheckIfReady()) {
		LevelLoader loader;
		loader.LoadLevel("../assets/json/map.json");
		lobbyState = false;
		playerManager->ManageMatchStatus(true);
		g_engine->ToggleMenu(false);
		MenuManager::GetInstance()->ClearMenu();
	}
}

void NetGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim();
	if(playerOne != NULL) playerOne->DrawOverlays(deltaTime);
	if(playerOne != NULL) g_engine->drawManaAndHealth(playerOne->GetHP(), playerOne->GetMP());
	//f_engine->DebugDrawWorld();
	GraphicEngine::getInstance()->drawAllGUI();	// Draws the MENU (if one is activated)
}

void NetGame::setFps(){
	secondCounter += deltaTime;
	if(secondCounter >= 0.5){
		secondCounter = 0;
		std::string myFps = to_string(int(1/deltaTime));
		std::wstring wsTmp(myFps.begin(), myFps.end());
		g_engine->ChangeWindowName(wsTmp);
	}
}

float NetGame::GetTotalTime(){ return GraphicEngine::getInstance()->getTime(); }

float NetGame::GetDeltaTime(){ return deltaTime; }

void NetGame::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}

void NetGame::CreatePlayer(NetworkObject* nObject, bool isPlayerOne){
	if(!isServer && playerOne == NULL && isPlayerOne) {
		playerOne = (HumanPlayer*)playerManager->AddHumanPlayer();
		playerOne->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, playerOne, SPELL_PROJECTILE);
		spellManager->AddHechizo(1, playerOne, SPELL_FIRE);
		spellManager->AddHechizo(2, playerOne, SPELL_WALL);
		spellManager->AddHechizo(3, playerOne, SPELL_BLIZZARD);
	}
	else{
		Player* newPlayer = playerManager->AddHumanPlayer(false);
		newPlayer->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, newPlayer, SPELL_PROJECTILE);
		spellManager->AddHechizo(1, newPlayer, SPELL_FIRE);
		spellManager->AddHechizo(2, newPlayer, SPELL_WALL);
		spellManager->AddHechizo(3, newPlayer, SPELL_BLIZZARD);
	}
}

void NetGame::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = ALLIANCE_WIZARD;
	else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;

	if(whosWon != NO_ALLIANCE){
		MatchEnded(whosWon);
		if(!isServer) n_engine->GetClient()->EndMatch(whosWon);
	}
}

void NetGame::RestartMatch(){
	gameEnded = false;
	lobbyState = true;
	LevelLoader loader;
	loader.LoadLevel("../assets/json/Lobby.json");
	MenuManager::GetInstance()->ClearMenu();
	playerManager->ManageMatchStatus(false);
	
	if(playerOne != NULL) {
		g_engine->ToggleMenu(false);
		playerOne->ReturnToLobby();
	}
}

void NetGame::MatchEnded(Alliance winnerAlliance){
	if(!gameEnded){
		GraphicEngine::getInstance()->InitReceiver();
		gameEnded = true;
		playerManager->EraseAllCharacters();
		if(playerOne != NULL) {
			MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, (int)winnerAlliance);
			g_engine->ToggleMenu(true);
			playerOne->SetAllInput(UP);
		}
		else RestartMatch();
	}
}