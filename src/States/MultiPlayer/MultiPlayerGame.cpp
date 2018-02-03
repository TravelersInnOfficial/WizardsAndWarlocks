#include "MultiPlayerGame.h"
#include "MultiLobby.h"
#include "MultiMatch.h"

MultiPlayerGame::MultiPlayerGame(){
	// Motores
	g_engine		= GraphicEngine::getInstance();
	f_engine		= BulletEngine::GetInstance();
	n_engine		= NetworkEngine::GetInstance();
	// Managers
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	m_stateGame 	= new MultiLobby(this);

	debug 		= false;
	mute 		= false;
	captured 	= false;

	m_changeMode = 0;
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

void MultiPlayerGame::ReturnLobby(){
	m_changeMode = 1;
}

void MultiPlayerGame::StartGame(){
	m_changeMode = 2; // Modo Partida
}

void MultiPlayerGame::ChangeMode(){
	switch(m_changeMode){
		case 1:
			// Eliminamos el estado que haya actualmente
			delete m_stateGame;
			CleanGame();
			m_stateGame = new MultiLobby(this);
			playerManager->RestartMatchStatus();
			m_changeMode = 0;
			break;
		case 2:
			// Eliminamos el estado que haya actualmente
			delete m_stateGame;
			CleanGame();
			m_stateGame = new MultiMatch(this);
			playerManager->RestartMatchStatus();
			m_changeMode = 0;
			break;
	}
}

void MultiPlayerGame::CleanGame(){
	// Limpiamos los objetos
	objectManager->ClearMap();
	// Limpiamos los proyectiles
	bulletManager->DeleteAllProyectiles();
	// Limpiamos las trampas
	trapManager->ClearTraps();
	// Limpiamos los hechizos
	spellManager->ResetAllDieHechizo();
}

bool MultiPlayerGame::Input(){
	if(!g_engine->run()){
		return true;
	}

	if(g_engine->IsKeyPressed(KEY_ESCAPE)){
		StateManager::GetInstance()->PrepareStatus(STATE_MENU);
	}

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
	if(m_changeMode != 0){
		ChangeMode();
	}
	m_stateGame->Update(deltaTime);
}


void MultiPlayerGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();

	m_stateGame->Draw();

	if(debug){
		f_engine->DebugDrawWorld();
		//if(AL != NULL) AL->Debug();
	}

	g_engine->endScene();
}