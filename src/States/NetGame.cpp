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
		if(g_engine->ReadButtonPressed() == ENDMATCH_M_CONFIRM) RestartMatch();
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

	if(lobbyState){
		if(playerManager->CheckIfReady()) {
			LevelLoader loader;
			loader.LoadLevel("../assets/json/map.json");
			lobbyState = false;
			playerManager->ManageMatchStatus(true);
		}
	}
	else if (!gameEnded) CheckIfWon();
}

void NetGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim();
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

void NetGame::SetPlayerOne(NetworkObject* nObject){
	if(!isServer && playerOne == NULL) {
		playerOne = (HumanPlayer*)playerManager->AddHumanPlayer();
		playerOne->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, playerOne, SPELL_PROYECTIL);
		spellManager->AddHechizo(1, playerOne, SPELL_BASIC);
		spellManager->AddHechizo(2, playerOne, SPELL_DESPERIATONMURI);
		spellManager->AddHechizo(3, playerOne, SPELL_GUIVERNUMVENTUS);
	}
	else{
		Player* newPlayer = playerManager->AddHumanPlayer(false);
		newPlayer->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, newPlayer, SPELL_PROYECTIL);
		spellManager->AddHechizo(1, newPlayer, SPELL_BASIC);
		spellManager->AddHechizo(2, newPlayer, SPELL_DESPERIATONMURI);
		spellManager->AddHechizo(3, newPlayer, SPELL_GUIVERNUMVENTUS);
	}
}

void NetGame::CheckIfWon(){
	int whosWon = -1;

	if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = 0;
	else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = 1;

	if(whosWon != -1){
		GraphicEngine::getInstance()->InitReceiver();
		gameEnded = true;
		playerManager->EraseAllCharacters();
		if(playerOne != NULL) {
			MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, whosWon);
			g_engine->ToggleMenu(true);
			playerOne->SetAllInput(RELEASED);
		}
		else RestartMatch();
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