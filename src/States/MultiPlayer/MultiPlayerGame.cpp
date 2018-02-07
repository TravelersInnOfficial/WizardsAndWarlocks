#include "MultiPlayerGame.h"
#include "MultiLobby.h"
#include "MultiMatch.h"

MultiPlayerGame::MultiPlayerGame(){
	// Motores
	g_engine		= GraphicEngine::getInstance();
	f_engine		= BulletEngine::GetInstance();
	n_engine		= NetworkEngine::GetInstance();
	s_engine		= SoundSystem::getInstance();
	
	// Managers
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	// Creamos un nuevo MultiLobby
	m_stateGame 	= new MultiLobby(this);
	
	// Si estaba algun menu activado lo desactivamos
	g_engine->ToggleCameraMovement(true);
	g_engine->ToggleMenu(false);
	MenuManager::GetInstance()->ClearMenu();

	// Inicializamos nuestras variables
	debug 		= false;
	mute 		= false;
	captured 	= false;
	m_changeMode = 0;

	// Y creamos los eventos de sonido
	CreateSoundEvents();
}

MultiPlayerGame::~MultiPlayerGame(){
	delete m_stateGame;
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete trapManager;
	delete networkManager;
	n_engine->EndService();
}

// Ponemos el modo LOBBY en el siguiente Update
void MultiPlayerGame::ReturnLobby(){
	m_changeMode = 1;
}

// Ponemos el modo PARTIDA en el sigueinte Update
void MultiPlayerGame::StartGame(){
	m_changeMode = 2;
}

void MultiPlayerGame::ChangeMode(){
	switch(m_changeMode){
		case 1:
			// Eliminamos el estado que haya actualmente
			delete m_stateGame;
			CleanGame();

			// Creamos una nueva instancia del modo al que vamos a entrar
			m_stateGame = new MultiLobby(this);
			playerManager->RestartMatchStatus();

			// Reinicializamos el changeMode
			m_changeMode = 0;
			break;
		case 2:
			// Eliminamos el estado que haya actualmente
			delete m_stateGame;
			CleanGame();

			// Creamos una nueva instancia del modo al que vamos a entrar
			m_stateGame = new MultiMatch(this);
			playerManager->InitGame();

			// Reinicializamos el changeMode
			m_changeMode = 0;
			break;
	}
}

void MultiPlayerGame::CleanGame(){
	objectManager->ClearMap(); 				// Limpiamos los objetos
	bulletManager->DeleteAllProyectiles();	// Limpiamos los proyectiles
	trapManager->ClearTraps();				// Limpiamos las trampas
	spellManager->ResetAllDieHechizo();		// Limpiamos los hechizos
}

bool MultiPlayerGame::Input(){
	if(!g_engine->run()) return true;

	if(g_engine->IsKeyPressed(KEY_ESCAPE)) StateManager::GetInstance()->PrepareStatus(STATE_MENU);

	// DEBUG
	if(g_engine->IsKeyPressed(KEY_F1)) {
		MenuManager::GetInstance()->CreateMenu(NETDEBUG_M);
		debug = !debug;
	}

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

	return m_stateGame->Input();
}

void MultiPlayerGame::Update(float deltaTime){
	if(m_changeMode != 0) ChangeMode();
	m_stateGame->Update(deltaTime);
}


void MultiPlayerGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	m_stateGame->Draw();
	if(debug) f_engine->DebugDrawWorld();
	g_engine->endScene();
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void MultiPlayerGame::CreateSoundEvents() {
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

void MultiPlayerGame::PlayEvent(std::string event, vector3df pos) {
	s_engine->playEvent(soundEvents[event], pos);
}

void MultiPlayerGame::PlayEvent(std::string event) {
	s_engine->playEvent(soundEvents[event]);
}
