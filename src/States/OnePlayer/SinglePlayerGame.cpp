#include "SinglePlayerGame.h"
#include "./Lobby.h"
#include "./Match.h"

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

	m_stateGame 	= new Lobby(this);

	debug 		= false;
	mute 		= false;
	captured 	= false;
}

SinglePlayerGame::~SinglePlayerGame(){
	delete m_stateGame;
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete trapManager;
	delete senseManager;
}

void SinglePlayerGame::StartGame(){
	// Eliminamos el estado que haya actualmente
	delete m_stateGame;

	// Limpiamos el juego
	CleanGame();

	// Cargamos la partida
	m_stateGame = new Match(this);

	// Posicionamos los jugadores
	playerManager->RestartMatchStatus();
}

void SinglePlayerGame::ReturnLobby(){
	// Eliminamos el estado que haya actualmente
	delete m_stateGame;

	// Limpiamos el juego
	CleanGame();

	// Cargamos el Lobby
	m_stateGame = new Lobby(this);

	// Posicionamos los jugadores
	playerManager->RestartMatchStatus();
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

	return m_stateGame->Input();
}

void SinglePlayerGame::Update(float deltaTime){
	m_stateGame->Update(deltaTime);
}

void SinglePlayerGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();

	m_stateGame->Draw();

	if(debug){
		f_engine->DebugDrawWorld();
		//if(AL != NULL) AL->Debug();
	}

	g_engine->endScene();
}