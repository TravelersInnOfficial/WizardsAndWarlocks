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
	n_engine 		= NetworkEngine::GetInstance();
	s_engine 		= SoundSystem::getInstance();

	// Level
	LevelLoader loader;
	loader.LoadLevel("../assets/json/Lobby.json");
	lobbyState = true;
	gameEnded = false;
	debug = false;
	secondCounter = 0;

	if(n_engine->IsServerInit()) isServer = true;
	else if(n_engine->IsClientInit()) isServer = false;

	// Graphic Engine
	timeStart = g_engine->getTime() * 0.001;
	g_engine->addCameraSceneNodeFPS(120.f, 0.005);

	// Jugador
	playerOne = NULL;

	// Sound Engine
	createSoundEvents();

	playEvent(soundEvents["ghosts"], vector3df(-0.245, 1.14, 17.25));
	playEvent(soundEvents["waterdrops"], vector3df(-0.245, 1.20, 17.25));
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
	if(g_engine->IsKeyPressed(KEY_F1)){
		MenuManager::GetInstance()->CreateMenu(NETDEBUG_M);
		debug = !debug;
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
	
	if(g_engine->getActiveCamera() != NULL){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}

	playerManager->UpdatePlayers(true);

	networkManager->Update();
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	trapManager->Update(deltaTime);

	//if(lobbyState) playerManager->RespawnDeadPlayers();
	MenuManager::GetInstance()->UpdateNetDebug();
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

	if(playerOne != NULL){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->DrawOverlays(deltaTime);
		g_engine->drawManaAndHealth(playerOne->GetHP(), playerOne->GetMP());
		spellManager->DrawHUDSpells(playerOne);
		objectManager->DrawGrailGUI();
	}
	
	if(debug) f_engine->DebugDrawWorld();
	
	GraphicEngine::getInstance()->drawAllGUI();	// Draws the MENU (if one is activated)
}

void NetGame::setFps(){
	secondCounter += deltaTime;
	if(secondCounter >= 0.5){
		secondCounter = 0;
		std::string myFps = to_string(int(1/deltaTime));
		std::wstring wsTmp(myFps.begin(), myFps.end());
		g_engine->ChangeWindowName(L"Wizards And Warlocks Master v1.0 FPS:" + wsTmp);
	}
}

float NetGame::GetTotalTime(){ return GraphicEngine::getInstance()->getTime(); }

float NetGame::GetDeltaTime(){ return deltaTime; }

bool NetGame::GetLobbyState(){ return lobbyState; }

void NetGame::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}

void NetGame::CreatePlayer(NetworkObject* nObject, bool isPlayerOne, std::string name){
	Player* p;
	
	if(!isServer && playerOne == NULL && isPlayerOne) {
		playerOne = (HumanPlayer*)playerManager->AddHumanPlayer();
		playerOne->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, playerOne, SPELL_PROJECTILE);
		spellManager->AddHechizo(1, playerOne, SPELL_FIRE);
		spellManager->AddHechizo(2, playerOne, SPELL_WALL);
		spellManager->AddHechizo(3, playerOne, SPELL_BLIZZARD);
		p = (Player*) playerOne;
	}
	else{
		Player* newPlayer = playerManager->AddHumanPlayer(false);
		newPlayer->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, newPlayer, SPELL_PROJECTILE);
		spellManager->AddHechizo(1, newPlayer, SPELL_FIRE);
		spellManager->AddHechizo(2, newPlayer, SPELL_WALL);
		spellManager->AddHechizo(3, newPlayer, SPELL_BLIZZARD);
		p = (Player*) newPlayer;
	}

	if(name.length() > 0) p->SetName(name);
}

void NetGame::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(isServer){
		if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)){
			whosWon = ALLIANCE_WIZARD;
		}
		else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;
		
		if(whosWon != NO_ALLIANCE) {
			MatchEnded(whosWon);
		} 
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
	
	if(isServer) n_engine->GetServer()->EndMatch(winnerAlliance);

	if(!gameEnded){
		//Play sound event when you lose or win
		if (playerOne != NULL) {
			if (playerOne->GetAlliance() != winnerAlliance)  playEvent(soundEvents["defeat"]);
			else playEvent(soundEvents["victory"]);
		}
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

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void NetGame::createSoundEvents() {
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
void NetGame::playEvent(SoundEvent* event, vector3df pos) {
	SoundSystem::getInstance()->playEvent(event, pos);
}

void NetGame::playEvent(SoundEvent* event) {
	SoundSystem::getInstance()->playEvent(event);
}
