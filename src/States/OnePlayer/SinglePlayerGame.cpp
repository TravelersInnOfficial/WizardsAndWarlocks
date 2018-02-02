#include "SinglePlayerGame.h"
#include "Lobby.h"
#include "Match.h"

SinglePlayerGame::SinglePlayerGame(){
	m_stateGame 	= new Lobby(this);

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	senseManager	= RegionalSenseManager::GetInstance();

	g_engine		= GraphicEngine::getInstance();
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

	return m_stateGame->Input();
}

void SinglePlayerGame::Update(float deltaTime){
	m_stateGame->Update(deltaTime);
}

void SinglePlayerGame::Draw(){
	m_stateGame->Draw();
}