#include "SinglePlayerGame.h"
#include "Lobby.h"
#include "Match.h"

SinglePlayerGame::SinglePlayerGame(){
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	senseManager	= RegionalSenseManager::GetInstance();

	g_engine		= GraphicEngine::getInstance();
	f_engine		= BulletEngine::GetInstance();
	s_engine 		= SoundSystem::getInstance();

	m_stateGame 	= new Lobby(this);

	debug 		= false;
	mute 		= false;
	captured 	= false;

	m_changeMode	= 0;
	AL = NULL;

	CreateSoundEvents();
}

SinglePlayerGame::~SinglePlayerGame(){
	delete m_stateGame;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete spellManager;	// Tiene que eliminarse despues de el playerManager NECESARIO
	delete trapManager;
	delete senseManager;

	// Liberamos los sonidos que teniamos puestos
	std::map<std::string, SoundEvent*>::iterator it = soundEvents.begin();
	for(; it!=soundEvents.end(); it++){
		SoundEvent* even = it->second;
		even->release();
		delete even;
	}
}

void SinglePlayerGame::StartGame(){
	m_changeMode = 1;
}

void SinglePlayerGame::ReturnLobby(){
	m_changeMode = 2;
}


void SinglePlayerGame::ChangeMode(){
	switch(m_changeMode){
		case 1:
			delete m_stateGame;						// Eliminamos el estado que haya actualmente
			CleanGame();							// Limpiamos el juego
			m_stateGame = new Match(this);			// Cargamos la partida
			playerManager->InitGame();				// Posicionamos los jugadores
			m_changeMode = 0;
			break;
		case 2:
			delete m_stateGame;
			CleanGame();
			m_stateGame = new Lobby(this);
			playerManager->RestartMatchStatus();
			m_changeMode = 0;
			break;
	}
}

void SinglePlayerGame::CleanGame(){
	// Limpiamos los objetos
	objectManager->ClearMap();
	// Limpiamos los proyectiles
	bulletManager->DeleteAllProyectiles();
	// Limpiamos las trampas
	trapManager->ClearTraps();
	// Limpiamos los hechizos
	spellManager->ResetAllDieHechizo();

	// Si estaba algun menu activado lo desactivamos
	g_engine->ToggleCameraMovement(true);
	g_engine->ToggleMenu(false);
	MenuManager::GetInstance()->ClearMenu();
}

bool SinglePlayerGame::Input(){
	if(!g_engine->run()){
		return true;
	}

	if(g_engine->IsKeyPressed(KEY_ESCAPE)){
		StateManager::GetInstance()->PrepareStatus(STATE_MENU);
	}

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

	if(g_engine->IsKeyPressed(KEY_KEY_M)){
		AL = playerManager->AddAIPlayer();
	}

	return m_stateGame->Input();
}

void SinglePlayerGame::Update(float deltaTime){
	if(m_changeMode != 0){
		ChangeMode();
	}
	m_stateGame->Update(deltaTime);
}

void SinglePlayerGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();

	m_stateGame->Draw();

	if(debug){
		f_engine->DebugDrawWorld();
		if(AL != NULL) AL->Debug();
	}

	g_engine->endScene();
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void SinglePlayerGame::CreateSoundEvents() {
	//Create the sound events
	SoundEvent* defeat  = s_engine->createEvent("event:/Music/Defeat");
	SoundEvent* victory = s_engine->createEvent("event:/Music/Victory");
	SoundEvent* ghosts  = s_engine->createEvent("event:/Ambience/Ghosts");
	SoundEvent* waterDrops  = s_engine->createEvent("event:/Ambience/Water Drops");
	
	//Store them at the map
	soundEvents["defeat"]  = defeat;
	soundEvents["victory"] = victory;
	soundEvents["ghosts"]  = ghosts;
	soundEvents["waterdrops"]  = waterDrops;
	
}
void SinglePlayerGame::PlayEvent(std::string event, vector3df pos) {
	s_engine->playEvent(soundEvents[event], pos);
}

void SinglePlayerGame::PlayEvent(std::string event) {
	s_engine->playEvent(soundEvents[event]);
}
